
#include "render/comp.h"
#include "context.h"


//栅栏(fence)
//使用栅栏(fence)来对应用程序本身和渲染操作进行同步，可以调用vkWaitForFences函数查询状态
void RenderSystem::CreateFences(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& inFlightFences = globalInfoComp->inFlightFences;

	auto maxFrameInFlight = globalInfoComp->maxFrameInFlight;

	inFlightFences.resize(maxFrameInFlight);

	for (auto i = 0u; i < maxFrameInFlight; i++) {

		VkFenceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		//VkFenceCreateFlags 
		//	VK_FENCE_CREATE_SIGNALED_BIT 创建时候就是准备状态
		createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		auto ret = vkCreateFence(logicDevice, &createInfo, nullptr, &inFlightFences[i]);
		if (ret != VK_SUCCESS) {
			throw std::runtime_error("create fence error!");
		}
	}
}

void RenderSystem::DestroyFences(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& inFlightFences = globalInfoComp->inFlightFences;

	for (auto i = 0u; i < globalInfoComp->maxFrameInFlight; i++) {
		auto& inFlightFence = inFlightFences[i];

		vkDestroyFence(logicDevice, inFlightFence, nullptr);
	}
}