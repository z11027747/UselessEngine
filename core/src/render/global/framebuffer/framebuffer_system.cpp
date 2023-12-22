
#include <array>
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/pass/pass_system.h"
#include "render/global/framebuffer/framebuffer_system.h"
#include "render/image/image_comp.h"
#include "context.h"

namespace Render {

	void FramebufferSystem::Create(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& surfaceCapabilities = global->surfaceCapabilities;
		auto& renderPass = global->renderPass;
		auto& swapchainColorImage2Ds = global->swapchainColorImage2Ds;
		auto& swapchainDepthImage2Ds = global->swapchainDepthImage2Ds;

		auto imageCount = static_cast<uint32_t>(swapchainColorImage2Ds.size());

		auto& frameBuffers = global->frameBuffers;
		frameBuffers.resize(imageCount);

		for (auto i = 0u; i < imageCount; i++) {

			std::array<VkImageView, 2> attachments = {
				swapchainColorImage2Ds[i]->vkImageView,
				swapchainDepthImage2Ds[i]->vkImageView
			};

			VkFramebufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.width = surfaceCapabilities.currentExtent.width;
			createInfo.height = surfaceCapabilities.currentExtent.height;
			createInfo.renderPass = renderPass;
			createInfo.layers = 1;

			auto ret = vkCreateFramebuffer(logicalDevice, &createInfo, nullptr, &frameBuffers[i]);
			CheckRet(ret, "vkCreateFramebuffer");
		}
	}

	void FramebufferSystem::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& frameBuffers = global->frameBuffers;

		for (auto& frameBuffer : frameBuffers) {
			vkDestroyFramebuffer(logicalDevice, frameBuffer, nullptr);
		}
	}
}
