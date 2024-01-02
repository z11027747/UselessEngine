
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_logic.h"
#include "context.h"

namespace Render {

	void CmdSubmitLogic::Create(Context* context,
		std::function<void(VkCommandBuffer&)> doCmds
	) {
		auto& batchCmd = context->renderBatchCmd;

		if (batchCmd == nullptr) {
			batchCmd = std::make_shared<Cmd>();
		}

		CmdPoolLogic::Allocate(context,
			batchCmd, 1);

		Record(batchCmd->vkCmdBuffers.back(), doCmds);
	}

	void CmdSubmitLogic::Record(VkCommandBuffer& cmdBuffer,
		std::function<void(VkCommandBuffer&)> doCmds
	) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		auto beginRet = vkBeginCommandBuffer(cmdBuffer, &beginInfo);
		CheckRet(beginRet, "vkBeginCommandBuffer");

		doCmds(cmdBuffer);

		auto endRet = vkEndCommandBuffer(cmdBuffer);
		CheckRet(endRet, "vkEndCommandBuffer");
	}

	// ==== ToSystem
	void CmdSubmitLogic::UpdateBatch(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalQueue = global->logicalQueue;

		auto& batchCmd = context->renderBatchCmd;

		auto& cmdBuffers = batchCmd->vkCmdBuffers;
		auto cmdSize = static_cast<uint32_t>(cmdBuffers.size());

		if (cmdSize > 0u) {
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());
			submitInfo.pCommandBuffers = cmdBuffers.data();

			auto submitRet = vkQueueSubmit(logicalQueue, 1, &submitInfo, nullptr);
			CheckRet(submitRet, "vkQueueSubmit");

			vkQueueWaitIdle(logicalQueue);

			CmdPoolLogic::Free(context, batchCmd);
		}
	}
}