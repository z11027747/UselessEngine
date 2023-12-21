
#include "render/global/global_def.h"
#include "render/cmd/cmd_comp.h"
#include "render/device/device_comp.h"
#include "render/cmd/cmd_pool_system.h"
#include "render/cmd/cmd_submit_system.h"
#include "context.h"

namespace Render {


	void CmdSubmitSystem::Create(Context* context,
		std::function<void(VkCommandBuffer&)> doCmds
	) {
		auto buffer = CmdPoolSystem::AllocateBuffer(context,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		//VkCommandBufferUsageFlags 使用方式
		//	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT：指令缓冲在执行一次后，就被用来记录新的指令。
		//	VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT：这是一个只在一个渲染流程内使用的辅助指令缓冲。
		//	VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT：在指令缓冲等待执行时，仍然可以提交这一指令缓冲。
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		auto beginRet = vkBeginCommandBuffer(buffer, &beginInfo);
		CheckRet(beginRet, "vkBeginCommandBuffer");

		doCmds(buffer);

		auto endRet = vkEndCommandBuffer(buffer);
		CheckRet(endRet, "vkEndCommandBuffer");

		auto cmdSubmit = std::make_shared<CmdSubmit>();
		cmdSubmit->vkBuffer = buffer;

		auto cmdSubmitEO = std::make_shared<EngineObject>();
		cmdSubmitEO->AddComponent(cmdSubmit);

		context->renderCmdSubmitEOs.push_back(cmdSubmitEO);
	}

	void CmdSubmitSystem::Update(Context* context) {

		auto& cmdSubmitEOs = context->renderCmdSubmitEOs;

		auto vkBufferCount = static_cast<uint32_t>(cmdSubmitEOs.size());
		std::vector<VkCommandBuffer> vkBuffers(vkBufferCount);

		for (auto i = 0; i < cmdSubmitEOs.size(); i++) {
			auto& cmdSubmitEO = cmdSubmitEOs[i];

			auto cmdSubmit = cmdSubmitEO->GetComponent<CmdSubmit>();

			auto& vkBuffer = cmdSubmit->vkBuffer;
			vkBuffers[i] = vkBuffer;

			cmdSubmitEO->RemoveComponent<CmdSubmit>();
			cmdSubmitEOs.erase(cmdSubmitEOs.begin() + i);
			i--;
		}

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = vkBufferCount;
		submitInfo.pCommandBuffers = vkBuffers.data();

		auto& renderGlobalEO = context->renderGlobalEO;
		auto deivce = renderGlobalEO->GetComponent<Device>();
		auto& logicalQueue = deivce->logicalQueue;

		auto submitRet = vkQueueSubmit(logicalQueue, 1, &submitInfo, nullptr);
		if (submitRet != VK_SUCCESS) {
			throw std::runtime_error("submit error!");
		}

		//直接等待传输操作完成
		vkQueueWaitIdle(logicalQueue);

	}
}