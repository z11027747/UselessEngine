
#include <array>
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/pass/pass_system.h"
#include "render/global/framebuffer/framebuffer_system.h"
#include "render/image/image_comp.h"
#include "render/cmd/cmd_comp.h"
#include "render/cmd/cmd_pool_system.h"
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

		global->maxFrameInFlight = imageCount;

		CreateFences(context);
		CreateSemaphores(context);
	}

	void FramebufferSystem::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& frameBuffers = global->frameBuffers;

		for (auto& frameBuffer : frameBuffers) {
			vkDestroyFramebuffer(logicalDevice, frameBuffer, nullptr);
		}

		DestroyFences(context);
		DestroySemaphores(context);
	}

	void FramebufferSystem::CreateFences(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& inFlightFences = global->inFlightFences;
		inFlightFences.resize(maxFrameInFlight);

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			VkFenceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			VkFence vkFence;
			auto ret = vkCreateFence(logicalDevice, &createInfo, nullptr, &vkFence);
			CheckRet(ret, "vkCreateFence");

			inFlightFences[i] = vkFence;
		}
	}

	void FramebufferSystem::DestroyFences(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& inFlightFences = global->inFlightFences;
		for (const auto& inFlightFence : inFlightFences) {
			vkDestroyFence(logicalDevice, inFlightFence, nullptr);
		}
		inFlightFences.clear();
	}

	void FramebufferSystem::CreateSemaphores(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& imageAvailableSemaphores = global->imageAvailableSemaphores;
		auto& renderFinishedSemaphores = global->renderFinishedSemaphores;
		imageAvailableSemaphores.resize(maxFrameInFlight);
		renderFinishedSemaphores.resize(maxFrameInFlight);

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			VkSemaphoreCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			createInfo.flags = 0;

			VkSemaphore availableSemaphore;
			auto avaliableRet = vkCreateSemaphore(logicalDevice, &createInfo, nullptr, &availableSemaphore);
			CheckRet(avaliableRet, "vkCreateSemaphore-Avaliable");

			VkSemaphore finishedSemaphore;
			auto finishedRet = vkCreateSemaphore(logicalDevice, &createInfo, nullptr, &finishedSemaphore);
			CheckRet(finishedRet, "vkCreateSemaphore-Finished");

			imageAvailableSemaphores[i] = availableSemaphore;
			renderFinishedSemaphores[i] = finishedSemaphore;
		}
	}

	void FramebufferSystem::DestroySemaphores(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& imageAvailableSemaphores = global->imageAvailableSemaphores;
		auto& renderFinishedSemaphores = global->renderFinishedSemaphores;

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
		}

		imageAvailableSemaphores.clear();
		renderFinishedSemaphores.clear();
	}

}
