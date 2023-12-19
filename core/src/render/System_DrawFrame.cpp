
#include "render/comp.h"
#include "context.h"


//从交换链获取图像，提交指令缓冲执行渲染指令，将渲染结果呈现到屏幕上

void RenderSystem::DrawFrame(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& logicQueue = globalInfoComp->logicQueue;
	auto& swapchain = globalInfoComp->swapchain;
	auto& commandBuffers = globalInfoComp->commandBuffers;

	auto maxFrameInFlight = globalInfoComp->maxFrameInFlight;
	auto& currFrame = globalInfoComp->currFrame;

	auto& imageAvailableSemaphore = globalInfoComp->imageAvailableSemaphores[currFrame];
	auto& renderFinishedSemaphore = globalInfoComp->renderFinishedSemaphores[currFrame];
	auto& inFlightFence = globalInfoComp->inFlightFences[currFrame];

	auto waitFenceRet = vkWaitForFences(logicDevice, 1, &inFlightFence, true, UINT64_MAX);
	if (waitFenceRet != VK_SUCCESS) {
		throw std::runtime_error("waitFence error!");
	}
	vkResetFences(logicDevice, 1, &inFlightFence);

	//	1.从交换链获取一张图像

	//获取可用的交换链图像的索引
	uint32_t imageIndex;
	auto acquireNextRet = vkAcquireNextImageKHR(logicDevice, swapchain, UINT64_MAX,
		imageAvailableSemaphore, nullptr, &imageIndex);

	//需要在交换链必须被重建时调用recreateSwapChain函数重建交换链
	//	VK_ERROR_OUT_OF_DATE_KHR：交换链不能继续使用。通常发生在窗口大小改变后。
	//	VK_SUBOPTIMAL_KHR：交换链仍然可以使用，但表面属性已经不能准确匹配。
	if (acquireNextRet == VK_ERROR_OUT_OF_DATE_KHR
		|| acquireNextRet == VK_SUBOPTIMAL_KHR) {
		RecreateSwapchain(context);
		return;
	}
	else if (acquireNextRet != VK_SUCCESS) {
		throw std::runtime_error("acquire NextImage error!");
	}

	//	2.对帧缓冲附着执行指令缓冲中的渲染指令

	auto& currBuffer = globalInfoComp->commandBuffers[imageIndex];
	vkResetCommandBuffer(currBuffer, 0);

	RecordCommandBuffer(context, imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	//等待信号量
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailableSemaphore;

	//VkPipelineStageFlags 管线阶段
	//	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT：管线到达可以写入颜色附着的管线阶段
	VkPipelineStageFlags waitStages[] = {
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	};
	submitInfo.pWaitDstStageMask = waitStages;

	//指定实际被提交执行的指令缓冲对象
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &currBuffer;

	//结束信号量
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

	//提交指令缓冲给图形指令队列，可以同时提交很多命令
	auto submitRet = vkQueueSubmit(logicQueue, 1, &submitInfo, inFlightFence);
	if (acquireNextRet != VK_SUCCESS) {
		throw std::runtime_error("submit error!");
	}

	//	3.返回渲染后的图像到交换链进行呈现操作

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	//等待信号量
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphore;

	//指定交换链信息
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &imageIndex;

	auto presentRet = vkQueuePresentKHR(logicQueue, &presentInfo);
	if (presentRet == VK_ERROR_OUT_OF_DATE_KHR
		|| acquireNextRet == VK_SUBOPTIMAL_KHR) {
		RecreateSwapchain(context);
		return;
	}
	else if (presentRet != VK_SUCCESS) {
		throw std::runtime_error("present error!");
	}

	currFrame = (currFrame + 1) % maxFrameInFlight;
}

void RenderSystem::DrawWait(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	vkDeviceWaitIdle(logicDevice);
}