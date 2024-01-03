﻿
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "context.h"

namespace Render
{
	VkCommandBuffer CmdSubmitLogic::CreateAndBegin(Context *context)
	{
		auto cmdBuffer = CmdPoolLogic::CreateBuffer(context);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		auto beginRet = vkBeginCommandBuffer(cmdBuffer, &beginInfo);
		CheckRet(beginRet, "vkBeginCommandBuffer");

		return cmdBuffer;
	}

	void CmdSubmitLogic::End(Context *context, VkCommandBuffer &cmdBuffer)
	{
		auto endRet = vkEndCommandBuffer(cmdBuffer);
		CheckRet(endRet, "vkEndCommandBuffer");

		context->renderBatchCmdBuffers.push_back(cmdBuffer);
	}

	void CmdSubmitLogic::EndSingleTime(Context *context, VkCommandBuffer &cmdBuffer)
	{
		auto endRet = vkEndCommandBuffer(cmdBuffer);
		CheckRet(endRet, "vkEndCommandBuffer");

		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto &logicalQueue = global->logicalQueue;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;

		auto submitRet = vkQueueSubmit(logicalQueue, 1, &submitInfo, nullptr);
		CheckRet(submitRet, "vkQueueSubmit");

		vkQueueWaitIdle(logicalQueue);

		std::vector<VkCommandBuffer> cmdBuffers = {cmdBuffer};
		CmdPoolLogic::FreeBuffers(context, cmdBuffers);
	}

	// ==== ToSystem
	void CmdSubmitLogic::UpdateBatch(Context *context)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto &logicalQueue = global->logicalQueue;

		auto &batchCmdBuffers = context->renderBatchCmdBuffers;
		auto batchCmdBufferSize = static_cast<uint32_t>(batchCmdBuffers.size());

		if (batchCmdBufferSize > 0u)
		{
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = static_cast<uint32_t>(batchCmdBuffers.size());
			submitInfo.pCommandBuffers = batchCmdBuffers.data();

			auto submitRet = vkQueueSubmit(logicalQueue, 1, &submitInfo, nullptr);
			CheckRet(submitRet, "vkQueueSubmit");

			vkQueueWaitIdle(logicalQueue);

			CmdPoolLogic::FreeBuffers(context, batchCmdBuffers);
			batchCmdBuffers.clear();
		}
	}
}