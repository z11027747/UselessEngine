
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "context.h"

namespace Render {

	void CmdPoolLogic::Create(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto physicalQueueFamilyIndex = global->physicalQueueFamilyIndex;

		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = physicalQueueFamilyIndex;

		//VkCommandPoolCreateFlags	标记
		//	VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
		//			使用它分配的指令缓冲对象被频繁用来记录新的指令(使用这一标记可能会改变帧缓冲对象的内存分配策略)。
		//	VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
		//			指令缓冲对象之间相互独立，不会被一起重置。不使用这一标记，指令缓冲对象会被放在一起重置。
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VkCommandPool vkPool;
		auto ret = vkCreateCommandPool(logicalDevice, &createInfo, nullptr, &vkPool);
		CheckRet(ret, "vkCreateCommandPool");

		auto cmdPool = std::make_shared<CmdPool>();
		cmdPool->vkPool = vkPool;

		renderGlobalEO->AddComponent(cmdPool);
	}

	void CmdPoolLogic::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto cmdPool = renderGlobalEO->GetComponent<CmdPool>();
		vkDestroyCommandPool(logicalDevice, cmdPool->vkPool, nullptr);

		renderGlobalEO->RemoveComponent<CmdPool>();
	}

	//分配指令缓冲对象，使用它记录绘制指令
	std::vector<VkCommandBuffer> CmdPoolLogic::AllocateBuffers(Context* context,
		uint32_t size,
		VkCommandBufferLevel level
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto cmdPool = renderGlobalEO->GetComponent<CmdPool>();
		auto& vkPool = cmdPool->vkPool;

		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = vkPool;
		allocateInfo.commandBufferCount = size;

		//VkCommandBufferLevel 缓冲对象级别
		//	VK_COMMAND_BUFFER_LEVEL_PRIMARY：可以被提交到队列进行执行，但不能被其它指令缓冲对象调用。
		//	VK_COMMAND_BUFFER_LEVEL_SECONDARY：不能直接被提交到队列进行执行，但可以被主要指令缓冲对象调用执行。
		allocateInfo.level = level;

		std::vector<VkCommandBuffer> vkBuffers(size);
		auto ret = vkAllocateCommandBuffers(logicalDevice, &allocateInfo, vkBuffers.data());
		CheckRet(ret, "vkAllocateCommandBuffers");

		return vkBuffers;
	}

	void CmdPoolLogic::FreeBuffer(Context* context, VkCommandBuffer& buffer) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto cmdPool = renderGlobalEO->GetComponent<CmdPool>();
		auto& vkPool = cmdPool->vkPool;

		vkFreeCommandBuffers(logicalDevice, vkPool, 1, &buffer);
	}

	void CmdPoolLogic::ResetBuffer(
		VkCommandBuffer& buffer, VkCommandBufferResetFlags resetFlags) {

		auto ret = vkResetCommandBuffer(buffer, resetFlags);
		CheckRet(ret, "vkResetCommandBuffer");
	}
}