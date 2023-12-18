
#include "render/comp.h"
#include "context.h"


//创建渲染流程对象时指定使用的附着需要绑定在帧缓冲对象上使用
//帧缓冲对象引用了用于表示附着的VkImageView对象

void RenderSystem::CreateFrameBuffers(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapchainImages = globalInfoComp->swapchainImages;
	auto& swapchainImageViews = globalInfoComp->swapchainImageViews;
	auto& swapChainExtent = globalInfoComp->swapChainExtent;
	auto& renderPass = globalInfoComp->renderPass;

	int imageSize = swapchainImages.size();

	auto& swapchainFramebuffers = globalInfoComp->swapchainFrameBuffers;
	swapchainFramebuffers.resize(imageSize);

	for (auto i = 0; i < imageSize; i++) {

		VkImageView attachments[] = {
			swapchainImageViews[i]
		};

		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = attachments;
		createInfo.width = swapChainExtent.width;
		createInfo.height = swapChainExtent.height;
		createInfo.renderPass = renderPass;
		createInfo.layers = 1; //几个图像层数

		auto ret = vkCreateFramebuffer(logicDevice, &createInfo, nullptr, &swapchainFramebuffers[i]);
		if (ret != VK_SUCCESS) {
			throw std::runtime_error("create framebuffer error!");
		}
	}
}

void RenderSystem::DestroyFrameBuffers(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapchainFrameBuffers = globalInfoComp->swapchainFrameBuffers;

	for (auto& swapchainFrameBuffer : swapchainFrameBuffers) {
		vkDestroyFramebuffer(logicDevice, swapchainFrameBuffer, nullptr);
	}

}
