
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "context.h"

namespace Render {

	void CmdSubmitLogic::Create(Context* context,
		std::function<void(VkCommandBuffer&)> doCmds
	) {
		auto cmdBuffer = CmdPoolLogic::AllocateBuffer(context,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		Record(cmdBuffer, doCmds);

		auto& cmdSimpleEO = context->renderCmdSimpleEO;

		if (cmdSimpleEO == nullptr) {
			cmdSimpleEO = std::make_shared<EngineObject>();

			auto cmdSimple = std::make_shared<CmdSimple>();
			cmdSimpleEO->AddComponent(cmdSimple);
		}

		auto cmdSimple = cmdSimpleEO->GetComponent<CmdSimple>();
		cmdSimple->vkCmdBuffers.push_back(cmdBuffer);
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

	void CmdSubmitLogic::Update(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalQueue = global->logicalQueue;

		auto& cmdSimpleEO = context->renderCmdSimpleEO;
		auto cmdSimple = cmdSimpleEO->GetComponent<CmdSimple>();
		auto& vkCmdBuffers = cmdSimple->vkCmdBuffers;

		auto vkCmdBufferSize = static_cast<uint32_t>(vkCmdBuffers.size());;
		if (vkCmdBufferSize == 0u) {
			return;
		}

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = static_cast<uint32_t>(vkCmdBuffers.size());
		submitInfo.pCommandBuffers = vkCmdBuffers.data();

		auto submitRet = vkQueueSubmit(logicalQueue, 1, &submitInfo, nullptr);
		CheckRet(submitRet, "vkQueueSubmit");

		//直接等待传输操作完成
		vkQueueWaitIdle(logicalQueue);

		for (auto& vkCmdBuffer : vkCmdBuffers) {
			CmdPoolLogic::FreeBuffer(context, vkCmdBuffer);
		}
		vkCmdBuffers.clear();
	}
}