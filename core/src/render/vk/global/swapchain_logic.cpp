
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/image/image_logic.h"
#include "context.h"

namespace Render {

	void SwapchainLogic::Create(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& surface = global->surface;
		auto& surfaceCapabilities = global->surfaceCapabilities;
		auto& surfaceFormat = global->surfaceFormat;
		auto& surfacePresentMode = global->surfacePresentMode;

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
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform; //图形变换操作，可以反转之类的
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //忽略alpha通道
		swapchainCreateInfo.presentMode = surfacePresentMode;
		swapchainCreateInfo.clipped = true;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		VkSwapchainKHR vkwapchain;
		auto ret = vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &vkwapchain);
		CheckRet(ret, "vkCreateSwapchainKHR");

		global->swapchain = vkwapchain;

		CreateColorImageViews(context);
		CreateDepthImageViews(context);
	}

	void SwapchainLogic::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& swapchain = global->swapchain;

		vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);

		DestroyColorImageViews(context);
		DestroyDepthImageViews(context);
	}

	void SwapchainLogic::CreateColorImageViews(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& swapchain = global->swapchain;
		auto& currentExtent = global->surfaceCapabilities.currentExtent;

		uint32_t swapchainImageCount;
		vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, nullptr);

		std::vector<VkImage> swapchainImageViews(swapchainImageCount);
		vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, swapchainImageViews.data());

		auto& swapchainColorImage2Ds = global->swapchainColorImage2Ds;
		swapchainColorImage2Ds.resize(swapchainImageCount);

		for (auto i = 0u; i < swapchainImageCount; i++) {
			auto colorImage2D = std::make_shared<Image>();
			colorImage2D->fomat = global->surfaceFormat.format;
			colorImage2D->extent = { currentExtent.width, currentExtent.height, 0 };
			colorImage2D->vkImage = swapchainImageViews[i];

			ImageLogic::CreateView(context,
				colorImage2D, VK_IMAGE_ASPECT_COLOR_BIT, 1);

			swapchainColorImage2Ds[i] = std::move(colorImage2D);
		}
	}

	void SwapchainLogic::DestroyColorImageViews(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& swapchainColorImage2Ds = global->swapchainColorImage2Ds;

		for (auto& swapchainColorImage2D : swapchainColorImage2Ds) {
			ImageLogic::DestroyView(context,
				swapchainColorImage2D);
		}
		swapchainColorImage2Ds.clear();
	}

	void SwapchainLogic::CreateDepthImageViews(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& currentExtent = global->surfaceCapabilities.currentExtent;
		auto& swapchain = global->swapchain;

		global->depthImageFormat = VK_FORMAT_D32_SFLOAT;

		auto& swapchainColorImage2Ds = global->swapchainColorImage2Ds;
		auto swapchainImageCount = static_cast<uint32_t>(swapchainColorImage2Ds.size());

		auto& swapchainDepthImage2Ds = global->swapchainDepthImage2Ds;
		swapchainDepthImage2Ds.resize(swapchainImageCount);

		for (auto i = 0u; i < swapchainImageCount; i++) {
			ImageInfo image2dInfo = {
				global->depthImageFormat, { currentExtent.width, currentExtent.height, 0 },  VK_IMAGE_ASPECT_DEPTH_BIT,
				VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 0, 1,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
			};
			auto depthImage2d = ImageLogic::CreateByInfo(context, image2dInfo);

			swapchainDepthImage2Ds[i] = std::move(depthImage2d);
		}
	}

	void SwapchainLogic::DestroyDepthImageViews(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& swapchainDepthImage2Ds = global->swapchainDepthImage2Ds;

		for (auto& swapchainDepthImage2D : swapchainDepthImage2Ds) {
			ImageLogic::Destroy(context,
				swapchainDepthImage2D);
		}
		swapchainDepthImage2Ds.clear();
	}

}
