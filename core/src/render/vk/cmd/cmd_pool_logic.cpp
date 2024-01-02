
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_logic.h"
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
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VkCommandPool vkPool;
		auto ret = vkCreateCommandPool(logicalDevice, &createInfo, nullptr, &vkPool);
		CheckRet(ret, "vkCreateCommandPool");

		global->vkPool = vkPool;
	}

	void CmdPoolLogic::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyCommandPool(logicalDevice, global->vkPool, nullptr);
	}

	std::shared_ptr<Cmd> CmdPoolLogic::CreateCmd(Context* context) {
		auto cmd = std::make_shared<Cmd>();
		cmd->vkCmdBuffers = {};

		return cmd;
	}

	void CmdPoolLogic::Allocate(Context* context,
		std::shared_ptr<Cmd> cmd, uint32_t size
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& vkPool = global->vkPool;

		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = vkPool;
		allocateInfo.commandBufferCount = size;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		std::vector<VkCommandBuffer> vkCmdBuffers(size);
		auto ret = vkAllocateCommandBuffers(logicalDevice, &allocateInfo, vkCmdBuffers.data());
		CheckRet(ret, "vkAllocateCommandBuffers");

		cmd->vkCmdBuffers.insert(cmd->vkCmdBuffers.end(),
			vkCmdBuffers.begin(), vkCmdBuffers.end());
	}

	void CmdPoolLogic::Free(Context* context, std::shared_ptr<Cmd> cmd) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& vkPool = global->vkPool;

		auto& vkCmdBuffers = cmd->vkCmdBuffers;
		for (const auto& vkCmdBuffer : vkCmdBuffers) {
			vkFreeCommandBuffers(logicalDevice, vkPool, 1, &vkCmdBuffer);
		}
		vkCmdBuffers.clear();
	}

	std::shared_ptr<Cmd> CmdPoolLogic::CreateTempCmd(Context* context) {
		auto cmd = CreateCmd(context);
		context->renderTempCmds.push_back(cmd);
		return cmd;
	}

	void CmdPoolLogic::DestroyAllTemps(Context* context) {
		auto& tempCmds = context->renderTempCmds;
		for (const auto& tempCmd : tempCmds) {
			Free(context, tempCmd);
		}
		tempCmds.clear();
	}

	void CmdPoolLogic::ResetBuffer(
		VkCommandBuffer& buffer, VkCommandBufferResetFlags resetFlags) {

		auto ret = vkResetCommandBuffer(buffer, resetFlags);
		CheckRet(ret, "vkResetCommandBuffer");
	}
}