
#include "render/comp.h"
#include "context.h"


//信号量：来对一个指令队列内的操作或多个不同指令队列的操作进行同步
//	imageAvailableSemaphore：发出图像已经被获取，可以开始渲染的信号
//	renderFinishedSemaphore：发出渲染已经结果，可以开始呈现的信号
void RenderSystem::CreateSemaphores(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& imageAvailableSemaphores = globalInfoComp->imageAvailableSemaphores;
	auto& renderFinishedSemaphores = globalInfoComp->renderFinishedSemaphores;

	globalInfoComp->maxFrameInFlight = 2;
	globalInfoComp->currFrame = 0;

	imageAvailableSemaphores.resize(2);
	renderFinishedSemaphores.resize(2);

	for (auto i = 0; i < globalInfoComp->maxFrameInFlight; i++) {

		VkSemaphoreCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		//VkSemaphoreCreateFlags 
		//	VK_FENCE_CREATE_SIGNALED_BIT 创建时候就是准备状态
		createInfo.flags = 0;

		auto imageAvaliableRet = vkCreateSemaphore(logicDevice, &createInfo, nullptr, &imageAvailableSemaphores[i]);
		if (imageAvaliableRet != VK_SUCCESS) {
			throw std::runtime_error("create imageAvaliable semaphore error!");
		}

		auto renderFinishedRet = vkCreateSemaphore(logicDevice, &createInfo, nullptr, &renderFinishedSemaphores[i]);
		if (renderFinishedRet != VK_SUCCESS) {
			throw std::runtime_error("create renderFinished semaphore error!");
		}
	}
}

void RenderSystem::DestroySemaphores(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& imageAvailableSemaphores = globalInfoComp->imageAvailableSemaphores;
	auto& renderFinishedSemaphores = globalInfoComp->renderFinishedSemaphores;

	for (auto i = 0; i < globalInfoComp->maxFrameInFlight; i++) {
		auto& imageAvailableSemaphore = imageAvailableSemaphores[i];
		auto& renderFinishedSemaphore = renderFinishedSemaphores[i];

		vkDestroySemaphore(logicDevice, imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(logicDevice, renderFinishedSemaphore, nullptr);
	}
}