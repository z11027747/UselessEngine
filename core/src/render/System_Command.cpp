
#include "render/comp.h"
#include "context.h"


//Vulkan下的指令，比如绘制指令和内存传输指令并不是直接通过函数调用执行的。
//	需要先定义指令缓冲对象，然后执行操作，最后提交队列


//指令池：用于管理指令缓冲对象使用的内存，并负责指令缓冲对象的分配
void RenderSystem::CreateCommandPool(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto physicalDeviceGraphicsFamily = globalInfoComp->physicalDeviceGraphicsFamily;

	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = physicalDeviceGraphicsFamily;

	//VkCommandPoolCreateFlags	标记
	//	VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
	//			使用它分配的指令缓冲对象被频繁用来记录新的指令(使用这一标记可能会改变帧缓冲对象的内存分配策略)。
	//	VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	//			指令缓冲对象之间相互独立，不会被一起重置。不使用这一标记，指令缓冲对象会被放在一起重置。
	createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	auto ret = vkCreateCommandPool(logicDevice, &createInfo, nullptr, &globalInfoComp->commandPool);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create commandPool error!");
	}
}

void RenderSystem::DestroyCommandPool(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& commandPool = globalInfoComp->commandPool;

	vkDestroyCommandPool(logicDevice, commandPool, nullptr);
}

//分配指令缓冲对象，使用它记录绘制指令
//	需要为交换链中的每一个图像分配一个指令缓冲对象
void RenderSystem::AllocateCommandBuffer(Context* context, std::vector<VkCommandBuffer>& commandBuffers) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& commandPool = globalInfoComp->commandPool;

	uint32_t size = static_cast<uint32_t>(commandBuffers.size());

	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool = commandPool;
	allocateInfo.commandBufferCount = size;

	//VkCommandBufferLevel 缓冲对象级别
	//	VK_COMMAND_BUFFER_LEVEL_PRIMARY：可以被提交到队列进行执行，但不能被其它指令缓冲对象调用。
	//	VK_COMMAND_BUFFER_LEVEL_SECONDARY：不能直接被提交到队列进行执行，但可以被主要指令缓冲对象调用执行。
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	auto ret = vkAllocateCommandBuffers(logicDevice, &allocateInfo, commandBuffers.data());
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("allocate commandBuffers error!");
	}
}

void RenderSystem::FreeCommandBuffer(Context* context, std::vector<VkCommandBuffer>& commandBuffers) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& commandPool = globalInfoComp->commandPool;

	vkFreeCommandBuffers(logicDevice, commandPool,
		static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

void RenderSystem::RecordCommandBufferSingleTime(Context* context, std::function<void(VkCommandBuffer&)> doSomethings) {

	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& logicQueue = globalInfoComp->logicQueue;

	std::vector<VkCommandBuffer> commandBuffers(1);
	AllocateCommandBuffer(context, commandBuffers);

	auto& commandBuffer = commandBuffers[0];

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	//VkCommandBufferUsageFlags 使用方式
	//	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT：指令缓冲在执行一次后，就被用来记录新的指令。
	//	VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT：这是一个只在一个渲染流程内使用的辅助指令缓冲。
	//	VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT：在指令缓冲等待执行时，仍然可以提交这一指令缓冲。
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	auto beginRet = vkBeginCommandBuffer(commandBuffer, &beginInfo);
	if (beginRet != VK_SUCCESS) {
		throw std::runtime_error("begin commandBuffer error!");
	}

	doSomethings(commandBuffer);

	auto endRet = vkEndCommandBuffer(commandBuffer);
	if (endRet != VK_SUCCESS) {
		throw std::runtime_error("end commandBuffer error!");
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	auto submitRet = vkQueueSubmit(logicQueue, 1, &submitInfo, nullptr);
	if (submitRet != VK_SUCCESS) {
		throw std::runtime_error("submit error!");
	}

	//直接等待传输操作完成
	vkQueueWaitIdle(logicQueue);

	FreeCommandBuffer(context, commandBuffers);
}
