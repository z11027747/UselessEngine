
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_logic.h"
#include "context.h"

namespace Render
{

	void CmdSubmitLogic::Create(Context *context,
								std::function<void(VkCommandBuffer &)> doCmds)
	{
		auto cmdBuffer = CmdPoolLogic::CreateBuffer(context);
		Record(cmdBuffer, doCmds);
		context->renderBatchCmdBuffers.push_back(cmdBuffer);
	}

	void CmdSubmitLogic::Record(VkCommandBuffer &cmdBuffer,
								std::function<void(VkCommandBuffer &)> doCmds)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		auto beginRet = vkBeginCommandBuffer(cmdBuffer, &beginInfo);
		CheckRet(beginRet, "vkBeginCommandBuffer");

		doCmds(cmdBuffer);

		auto endRet = vkEndCommandBuffer(cmdBuffer);
		CheckRet(endRet, "vkEndCommandBuffer");
	}

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