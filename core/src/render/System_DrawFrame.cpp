
#include "render/comp.h"
#include "context.h"


//从交换链获取图像，提交指令缓冲执行渲染指令，将渲染结果呈现到屏幕上

void RenderSystem::DrawFrame(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& logicQueue = globalInfoComp->logicQueue;
	auto& swapchain = globalInfoComp->swapchain;
	auto& swapchainCommandBuffers = globalInfoComp->swapchainCommandBuffers;

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
		SetNeedRecreateSwapchain(context);
		return;
	}
	else if (acquireNextRet != VK_SUCCESS) {
		throw std::runtime_error("acquire NextImage error!");
	}

	//	2.对帧缓冲附着执行指令缓冲中的渲染指令

	auto& commandBuffer = globalInfoComp->swapchainCommandBuffers[imageIndex];
	vkResetCommandBuffer(commandBuffer, 0);

	RecordDrawCommandBuffer(context, imageIndex);

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
	submitInfo.pCommandBuffers = &commandBuffer;

	//结束信号量
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

	//提交指令缓冲给图形指令队列，可以同时提交很多命令
	auto submitRet = vkQueueSubmit(logicQueue, 1, &submitInfo, inFlightFence);
	if (submitRet != VK_SUCCESS) {
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
		SetNeedRecreateSwapchain(context);
		return;
	}
	else if (presentRet != VK_SUCCESS) {
		throw std::runtime_error("present error!");
	}

	currFrame = (currFrame + 1) % maxFrameInFlight;
}

void RenderSystem::WaitIdle(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	vkDeviceWaitIdle(logicDevice);
}

void RenderSystem::RecordDrawCommandBuffer(Context* context, uint32_t index) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& renderPass = globalInfoComp->renderPass;
	auto& graphicsPipeline = globalInfoComp->graphicsPipeline;
	auto& swapChainExtent = globalInfoComp->swapChainExtent;
	auto& swapchainFrameBuffers = globalInfoComp->swapchainFrameBuffers;
	auto& swapchainCommandBuffers = globalInfoComp->swapchainCommandBuffers;

	auto& commandBuffer = globalInfoComp->swapchainCommandBuffers[index];

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	//VkCommandBufferUsageFlags 使用方式
	//	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT：指令缓冲在执行一次后，就被用来记录新的指令。
	//	VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT：这是一个只在一个渲染流程内使用的辅助指令缓冲。
	//	VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT：在指令缓冲等待执行时，仍然可以提交这一指令缓冲。
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	//开始记录指令到缓冲
	auto beginRet = vkBeginCommandBuffer(commandBuffer, &beginInfo);
	if (beginRet != VK_SUCCESS) {
		throw std::runtime_error("begin commandBuffer error!");
	}

	//开始渲染流程
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = swapchainFrameBuffers[index];

	//渲染区域
	renderPassBeginInfo.renderArea.offset = { 0, 0 };
	renderPassBeginInfo.renderArea.extent = swapChainExtent;
	renderPassBeginInfo.clearValueCount = 1;

	//清除值
	VkClearValue clearValue = { 0.0f,0.0f,0.0f,1.0f };
	renderPassBeginInfo.pClearValues = &clearValue;

	//VkSubpassContents 指定渲染流程如何提供绘制指令的标记
	//	VK_SUBPASS_CONTENTS_INLINE：所有要执行的指令都在主要指令缓冲中，没有辅助指令缓冲需要执行。
	//	VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS：有来自辅助指令缓冲的指令需要执行。
	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	//绑定图形管线
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	//绑定顶点缓冲
	auto& vertices = globalInfoComp->vertices;
	auto& vertexBuffer = globalInfoComp->vertexBuffer;

	//	firstBinding 偏移
	//	bindingCount 数量
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	//绘制对象
	//	vertexCount：指定x个顶点用于三角形的绘制
	//	instanceCount：用于实例渲染，为1时表示不进行实例渲染
	//	firstVertex：用于定义着色器变量gl_VertexIndex的值
	//	firstInstance：用于定义着色器变量gl_InstanceIndex的值
	vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	auto endRet = vkEndCommandBuffer(commandBuffer);
	if (endRet != VK_SUCCESS) {
		throw std::runtime_error("end commandBuffer error!");
	}

}