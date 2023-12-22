
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/cmd/cmd_comp.h"
#include "render/cmd/cmd_pool_system.h"
#include "render/cmd/cmd_submit_system.h"
#include "context.h"

namespace Render {

	void CmdSubmitSystem::Create(Context* context,
		std::function<void(VkCommandBuffer&)> doCmds
	) {
		auto cmdBuffer = RecordCmd(context, doCmds);

		auto& cmdSubmitEO = context->renderCmdSubmitEO;

		if (cmdSubmitEO == nullptr) {
			cmdSubmitEO = std::make_shared<EngineObject>();

			auto cmdSubmit = std::make_shared<CmdSubmit>();
			cmdSubmitEO->AddComponent(cmdSubmit);
		}

		auto cmdSubmit = cmdSubmitEO->GetComponent<CmdSubmit>();
		cmdSubmit->vkCmdBuffers.push_back(cmdBuffer);
	}

	void CmdSubmitSystem::CreateAsync(Context* context,
		std::function<void(VkCommandBuffer&)> doCmds,
		VkSemaphore& waitSemaphore, VkSemaphore& signalSemaphore
	) {
		auto cmdBuffer = RecordCmd(context, doCmds);

		auto& cmdSubmitSemaphoreEOs = context->renderCmdSubmitSemaphoreEOs;
		for (const auto& cmdSubmitSemaphoreEO : cmdSubmitSemaphoreEOs) {
			auto cmdSubmitSemaphore = cmdSubmitSemaphoreEO->GetComponent<CmdSubmitSemaphore>();
			if (waitSemaphore == cmdSubmitSemaphore->waitSemaphore
				&& signalSemaphore == cmdSubmitSemaphore->signalSemaphore) {

				cmdSubmitSemaphore->vkCmdBuffers.push_back(cmdBuffer);
				return;

			}
		}
		auto newCmdSubmitSemaphore = std::make_shared<CmdSubmitSemaphore>();
		newCmdSubmitSemaphore->vkCmdBuffers.push_back(cmdBuffer);
		newCmdSubmitSemaphore->waitSemaphore = waitSemaphore;
		newCmdSubmitSemaphore->signalSemaphore = signalSemaphore;

		auto newCmdSubmitSeaphoreEO = std::make_shared<EngineObject>();
		newCmdSubmitSeaphoreEO->AddComponent(newCmdSubmitSemaphore);

		cmdSubmitSemaphoreEOs.push_back(newCmdSubmitSeaphoreEO);
	}

	VkCommandBuffer CmdSubmitSystem::RecordCmd(Context* context
		, std::function<void(VkCommandBuffer&)> doCmds
	) {
		auto cmdBuffer = CmdPoolSystem::AllocateBuffer(context,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		//VkCommandBufferUsageFlags 使用方式
		//	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT：指令缓冲在执行一次后，就被用来记录新的指令。
		//	VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT：这是一个只在一个渲染流程内使用的辅助指令缓冲。
		//	VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT：在指令缓冲等待执行时，仍然可以提交这一指令缓冲。
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		auto beginRet = vkBeginCommandBuffer(cmdBuffer, &beginInfo);
		CheckRet(beginRet, "vkBeginCommandBuffer");

		doCmds(cmdBuffer);

		auto endRet = vkEndCommandBuffer(cmdBuffer);
		CheckRet(endRet, "vkEndCommandBuffer");

		return cmdBuffer;
	}

	void CmdSubmitSystem::Update(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalQueue = global->logicalQueue;

		auto& cmdSubmitEO = context->renderCmdSubmitEO;
		auto cmdSubmit = cmdSubmitEO->GetComponent<CmdSubmit>();
		auto& vkCmdBuffers = cmdSubmit->vkCmdBuffers;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = static_cast<uint32_t>(vkCmdBuffers.size());
		submitInfo.pCommandBuffers = vkCmdBuffers.data();

		auto submitRet = vkQueueSubmit(logicalQueue, 1, &submitInfo, nullptr);
		CheckRet(submitRet, "vkQueueSubmit");

		//直接等待传输操作完成
		vkQueueWaitIdle(logicalQueue);
	}

	void CmdSubmitSystem::UpdateSemaphore(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalQueue = global->logicalQueue;

		auto& cmdSubmitSemaphoreEOs = context->renderCmdSubmitSemaphoreEOs;
		auto submitCount = static_cast<uint32_t>(cmdSubmitSemaphoreEOs.size());
		std::vector<VkSubmitInfo> submitInfos(submitCount);

		for (auto i = 0u; i < submitCount; i++) {

			auto& cmdSubmitSemaphoreEO = cmdSubmitSemaphoreEOs[i];
			auto cmdSubmitSemaphore = cmdSubmitSemaphoreEO->GetComponent<CmdSubmitSemaphore>();

			auto& vkCmdBuffers = cmdSubmitSemaphore->vkCmdBuffers;
			auto& waitSemaphore = cmdSubmitSemaphore->waitSemaphore;
			auto& signalSemaphore = cmdSubmitSemaphore->signalSemaphore;

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = static_cast<uint32_t>(vkCmdBuffers.size());
			submitInfo.pCommandBuffers = vkCmdBuffers.data();
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &waitSemaphore;
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &signalSemaphore;
			submitInfos.push_back(submitInfo);
		}

		auto submitRet = vkQueueSubmit(logicalQueue, submitCount, submitInfos.data(), nullptr);
		CheckRet(submitRet, "vkQueueSubmit");

	}
}