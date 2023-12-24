
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/cmd/cmd_comp.h"
#include "render/cmd/cmd_pool_system.h"
#include "render/cmd/cmd_submit_system.h"
#include "context.h"

namespace Render {

	void CmdSubmitSystem::CreateSimple(Context* context,
		std::function<void(VkCommandBuffer&)> doCmds
	) {
		auto cmdBuffer = CmdPoolSystem::AllocateBuffer(context,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		RecordCmd(cmdBuffer, doCmds);

		auto& cmdSimpleEO = context->renderCmdSimpleEO;

		if (cmdSimpleEO == nullptr) {
			cmdSimpleEO = std::make_shared<EngineObject>();

			auto cmdSimple = std::make_shared<CmdSimple>();
			cmdSimpleEO->AddComponent(cmdSimple);
		}

		auto cmdSimple = cmdSimpleEO->GetComponent<CmdSimple>();
		cmdSimple->vkCmdBuffers.push_back(cmdBuffer);
	}

	void CmdSubmitSystem::RecordCmd(VkCommandBuffer& cmdBuffer,
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

	void CmdSubmitSystem::Update(Context* context) {
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
			CmdPoolSystem::FreeBuffer(context, vkCmdBuffer);
		}
		vkCmdBuffers.clear();
	}
}