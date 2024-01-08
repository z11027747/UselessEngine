
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "context.h"

namespace Render
{
	void CmdPoolLogic::Create(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;
		auto physicalQueueFamilyIndex = global->physicalQueueFamilyIndex;

		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = physicalQueueFamilyIndex;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VkCommandPool vkPool;
		auto ret = vkCreateCommandPool(logicalDevice, &createInfo, nullptr, &vkPool);
		CheckRet(ret, "vkCreateCommandPool");

		global->vkPool = vkPool;
	}

	void CmdPoolLogic::Destroy(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;

		vkDestroyCommandPool(logicalDevice, global->vkPool, nullptr);
	}

	std::vector<VkCommandBuffer> CmdPoolLogic::CreateBuffers(Context *context,
															 uint32_t count)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &vkPool = global->vkPool;

		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = vkPool;
		allocateInfo.commandBufferCount = count;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		std::vector<VkCommandBuffer> vkCmdBuffers(count);
		auto ret = vkAllocateCommandBuffers(logicalDevice, &allocateInfo, vkCmdBuffers.data());
		CheckRet(ret, "vkAllocateCommandBuffers");

		return vkCmdBuffers;
	}

	VkCommandBuffer CmdPoolLogic::CreateBuffer(Context *context)
	{
		return CreateBuffers(context, 1)[0];
	}

	VkCommandBuffer CmdPoolLogic::CreateTempBuffer(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto cmdBuffer = CreateBuffer(context);
		global->tempCmdBuffers.push_back(cmdBuffer);
		return cmdBuffer;
	}

	void CmdPoolLogic::DestroyAllTempBuffers(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &tempCmdBuffers = global->tempCmdBuffers;
		if (tempCmdBuffers.size() > 0)
		{
			FreeBuffers(context, tempCmdBuffers);
			tempCmdBuffers.clear();
		}
	}

	void CmdPoolLogic::FreeBuffers(Context *context, std::vector<VkCommandBuffer> &cmdBuffers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &vkPool = global->vkPool;

		vkFreeCommandBuffers(logicalDevice, vkPool, static_cast<uint32_t>(cmdBuffers.size()), cmdBuffers.data());
	}

	void CmdPoolLogic::ResetBuffer(
		VkCommandBuffer &buffer, VkCommandBufferResetFlags resetFlags)
	{
		auto ret = vkResetCommandBuffer(buffer, resetFlags);
		CheckRet(ret, "vkResetCommandBuffer");
	}
}