
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "engine_object.hpp"
#include "context.hpp"
#include "common/log.hpp"

namespace Render
{
	void SwapchainLogic::Create(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &surface = global->surface;
		auto &surfaceCapabilities = global->surfaceCapabilities;
		auto surfaceFormat = global->surfaceFormat;
		auto &surfacePresentMode = global->surfacePresentMode;

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount + 1;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = surfacePresentMode;
		swapchainCreateInfo.clipped = true;
		swapchainCreateInfo.oldSwapchain = nullptr;

		VkSwapchainKHR vkwapchain;
		auto ret = vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &vkwapchain);
		CheckRet(ret, "vkCreateSwapchainKHR");

		global->swapchain = vkwapchain;
	}

	void SwapchainLogic::CreateImageViews(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &swapchain = global->swapchain;
		auto surfaceFormat = global->surfaceFormat;
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		uint32_t swapchainImageCount;
		vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, nullptr);

		std::vector<VkImage> swapchainImages(swapchainImageCount);
		vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, swapchainImages.data());

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			auto swapchainImage2d = std::make_shared<Image>();
			swapchainImage2d->format = surfaceFormat.format;
			swapchainImage2d->extent = {currentExtent.width, currentExtent.height, 0};
			swapchainImage2d->aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			swapchainImage2d->layout = VK_IMAGE_LAYOUT_UNDEFINED;

			swapchainImage2d->vkImage = swapchainImages[i];

			ImageLogic::CreateView(context,
								   swapchainImage2d,
								   VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, 1, 1);

			global->swapchainImages.push_back(swapchainImage2d);
		}
		global->swapchainImageCount = swapchainImageCount;
		global->swapchainImageIndexs.resize(global->maxConcurrentFrame);
	}

	void SwapchainLogic::DestroyImageViews(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &swapchainImages = global->swapchainImages;

		for (auto &swapchainImage : swapchainImages)
		{
			ImageLogic::DestroyView(context,
									swapchainImage);
		}
	}

	void SwapchainLogic::Destroy(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		auto &swapchain = global->swapchain;
		vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
	}

	void SwapchainLogic::CreateFences(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		auto maxConcurrentFrame = global->maxConcurrentFrame;
		for (auto i = 0u; i < maxConcurrentFrame; i++)
		{
			VkFenceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			VkFence vkFence;
			auto ret = vkCreateFence(logicalDevice, &createInfo, nullptr, &vkFence);
			CheckRet(ret, "vkCreateFence");

			global->waitFences.push_back(vkFence);
		}
	}

	void SwapchainLogic::DestroyFences(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		auto &waitFences = global->waitFences;
		for (const auto &inFlightFence : waitFences)
		{
			vkDestroyFence(logicalDevice, inFlightFence, nullptr);
		}
	}

	void SwapchainLogic::CreateSemaphores(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		auto maxConcurrentFrame = global->maxConcurrentFrame;
		for (auto i = 0u; i < maxConcurrentFrame; i++)
		{
			VkSemaphoreCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			createInfo.flags = 0;

			VkSemaphore availableSemaphore;
			auto avaliableRet = vkCreateSemaphore(logicalDevice, &createInfo, nullptr, &availableSemaphore);
			CheckRet(avaliableRet, "vkCreateSemaphore-Avaliable");

			VkSemaphore finishedSemaphore;
			auto finishedRet = vkCreateSemaphore(logicalDevice, &createInfo, nullptr, &finishedSemaphore);
			CheckRet(finishedRet, "vkCreateSemaphore-Finished");

			global->presentCompleteSemaphores.push_back(availableSemaphore);
			global->renderCompleteSemaphores.push_back(finishedSemaphore);
		}
	}

	void SwapchainLogic::DestroySemaphores(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		auto &presentCompleteSemaphores = global->presentCompleteSemaphores;
		auto &renderCompleteSemaphore = global->renderCompleteSemaphores;

		auto maxConcurrentFrame = global->maxConcurrentFrame;
		for (auto i = 0u; i < maxConcurrentFrame; i++)
		{
			vkDestroySemaphore(logicalDevice, presentCompleteSemaphores[i], nullptr);
			vkDestroySemaphore(logicalDevice, renderCompleteSemaphore[i], nullptr);
		}
	}

	void SwapchainLogic::WaitFence(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto currFrame = global->currFrame;
		auto &waitFence = global->waitFences[currFrame];

		auto waitFenceRet = vkWaitForFences(logicalDevice, 1, &waitFence, true, UINT64_MAX);
		CheckRet(waitFenceRet, "vkWaitForFences");

		vkResetFences(logicalDevice, 1, &waitFence);
	}

	void SwapchainLogic::AcquireImageIndex(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		auto &swapchain = global->swapchain;

		auto currFrame = global->currFrame;
		auto &imageIndex = global->swapchainImageIndexs[currFrame];
		auto &presentCompleteSemaphore = global->presentCompleteSemaphores[currFrame];

		auto acquireNextRet = vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX,
													presentCompleteSemaphore, nullptr, &imageIndex);
		CheckRet(acquireNextRet, "vkAcquireNextImageKHR");
	}

	void SwapchainLogic::AllocateCmd(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto swapchainImageCount = global->swapchainImageCount;

		global->swapchainCmdBuffers = CmdPoolLogic::CreateBuffers(context, swapchainImageCount);
	}

	VkCommandBuffer &SwapchainLogic::BeginCmd(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();

		auto currFrame = global->currFrame;
		auto &swapchainCmdBuffer = global->swapchainCmdBuffers[currFrame];

		vkResetCommandBuffer(swapchainCmdBuffer, 0);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		auto beginRet = vkBeginCommandBuffer(swapchainCmdBuffer, &beginInfo);
		CheckRet(beginRet, "vkBeginCommandBuffer");

		return swapchainCmdBuffer;
	}

	void SwapchainLogic::EndAndSubmitCmd(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalQueue = global->logicalQueue;

		auto currFrame = global->currFrame;
		auto &swapchainCmdBuffer = global->swapchainCmdBuffers[currFrame];
		auto &waitFence = global->waitFences[currFrame];
		auto &presentCompleteSemaphore = global->presentCompleteSemaphores[currFrame];
		auto &renderCompleteSemaphore = global->renderCompleteSemaphores[currFrame];

		auto endRet = vkEndCommandBuffer(swapchainCmdBuffer);
		CheckRet(endRet, "vkEndCommandBuffer");

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &presentCompleteSemaphore;
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &swapchainCmdBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderCompleteSemaphore;

		auto submitRet = vkQueueSubmit(logicalQueue, 1, &submitInfo, waitFence);
		CheckRet(submitRet, "vkQueueSubmit");
	}

	void SwapchainLogic::Present(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalQueue = global->logicalQueue;
		auto &swapchain = global->swapchain;

		auto maxConcurrentFrame = global->maxConcurrentFrame;
		auto &currFrame = global->currFrame;
		auto &imageIndex = global->swapchainImageIndexs[currFrame];
		auto &renderCompleteSemaphore = global->renderCompleteSemaphores[currFrame];

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderCompleteSemaphore;

		auto presentRet = vkQueuePresentKHR(logicalQueue, &presentInfo);
		CheckRet(presentRet, "vkQueuePresentKHR");

		currFrame = (currFrame + 1) % maxConcurrentFrame;
	}

}
