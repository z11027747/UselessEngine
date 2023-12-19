
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
void RenderSystem::CreateCommandBuffers(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapchainFrameBuffers = globalInfoComp->swapchainFrameBuffers;

	int size = swapchainFrameBuffers.size();

	auto& commandPool = globalInfoComp->commandPool;
	auto& commandBuffers = globalInfoComp->commandBuffers;
	commandBuffers.resize(size);

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

void RenderSystem::RecordCommandBuffer(Context* context, uint32_t index) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& renderPass = globalInfoComp->renderPass;
	auto& graphicsPipeline = globalInfoComp->graphicsPipeline;
	auto& swapChainExtent = globalInfoComp->swapChainExtent;
	auto& swapchainFrameBuffers = globalInfoComp->swapchainFrameBuffers;
	auto& commandBuffers = globalInfoComp->commandBuffers;

	auto& currBuffer = globalInfoComp->commandBuffers[index];

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	//VkCommandBufferUsageFlags 使用方式
	//	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT：指令缓冲在执行一次后，就被用来记录新的指令。
	//	VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT：这是一个只在一个渲染流程内使用的辅助指令缓冲。
	//	VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT：在指令缓冲等待执行时，仍然可以提交这一指令缓冲。
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	//开始记录指令到缓冲
	auto beginRet = vkBeginCommandBuffer(currBuffer, &beginInfo);
	if (beginRet != VK_SUCCESS) {
		throw std::runtime_error("begin commandBuffer error!");
	}

	//开始渲染流程
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = swapchainFrameBuffers[index];

	//渲染区域
	renderPassBeginInfo.renderArea.offset = { 0, 0 };
	renderPassBeginInfo.renderArea.extent = swapChainExtent;
	renderPassBeginInfo.clearValueCount = 1;

	//清除值
	VkClearValue clearValue = { 0.0f,0.0f,0.0f,1.0f };
	renderPassBeginInfo.pClearValues = &clearValue;

	//VkSubpassContents 指定渲染流程如何提供绘制指令的标记
	//	VK_SUBPASS_CONTENTS_INLINE：所有要执行的指令都在主要指令缓冲中，没有辅助指令缓冲需要执行。
	//	VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS：有来自辅助指令缓冲的指令需要执行。
	vkCmdBeginRenderPass(currBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	//绑定图形管线
	vkCmdBindPipeline(currBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	//绑定顶点缓冲
	auto& vertices = globalInfoComp->vertices;
	auto& vertexBuffer = globalInfoComp->vertexBuffer;

	//	firstBinding 偏移
	//	bindingCount 数量
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(currBuffer, 0, 1, vertexBuffers, offsets);

	//绘制对象
	//	vertexCount：指定x个顶点用于三角形的绘制
	//	instanceCount：用于实例渲染，为1时表示不进行实例渲染
	//	firstVertex：用于定义着色器变量gl_VertexIndex的值
	//	firstInstance：用于定义着色器变量gl_InstanceIndex的值
	vkCmdDraw(currBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

	vkCmdEndRenderPass(currBuffer);

	auto endRet = vkEndCommandBuffer(currBuffer);
	if (endRet != VK_SUCCESS) {
		throw std::runtime_error("end commandBuffer error!");
	}

}