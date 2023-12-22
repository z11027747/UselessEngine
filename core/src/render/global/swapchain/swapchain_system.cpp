
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/global/swapchain/swapchain_system.h"
#include "render/image/image2d_system.h"
#include "context.h"

namespace Render {

	void SwapchainSystem::Create(Context* context) {


	}

	void SwapchainSystem::CreateColorImageViews(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& swapchain = global->swapchain;

		uint32_t swapchainImageCount;
		vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, nullptr);

		std::vector<VkImage> swapchainImageViews(swapchainImageCount);
		vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, swapchainImageViews.data());

		auto& swapchainColorImage2Ds = global->swapchainColorImage2Ds;
		swapchainColorImage2Ds.resize(swapchainImageCount);

		for (auto i = 0u; i < swapchainImageCount; i++) {
			auto colorImage2D = std::make_shared<Image2D>();
			colorImage2D->fomat = global->surfaceFormat.format;
			colorImage2D->extent = global->surfaceCapabilities.currentExtent;
			colorImage2D->vkImage = swapchainImageViews[i];

			Image2DSystem::CreateImageView(context,
				colorImage2D, VK_IMAGE_ASPECT_COLOR_BIT);

			swapchainColorImage2Ds[i] = std::move(colorImage2D);
		}
	}

	void SwapchainSystem::CreateDepthImageViews(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& surfaceCapabilities = global->surfaceCapabilities;
		auto& swapchain = global->swapchain;

		global->depthImageFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

		auto& swapchainColorImage2Ds = global->swapchainColorImage2Ds;
		auto swapchainImageCount = static_cast<uint32_t>(swapchainColorImage2Ds.size());

		for (auto i = 0u; i < swapchainImageCount; i++) {

			auto depthImage2D = Image2DSystem::Create(context,
				global->depthImageFormat, surfaceCapabilities.currentExtent,
				VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			Image2DSystem::CreateImageView(context,
				depthImage2D, VK_IMAGE_ASPECT_DEPTH_BIT);

			Image2DSystem::TransitionLayout(context,
				depthImage2D,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				VK_IMAGE_ASPECT_DEPTH_BIT,
				0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
			);

			swapchainColorImage2Ds[i] = std::move(depthImage2D);
		}
	}

	void SwapchainSystem::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& swapchain = global->swapchain;

		auto& swapchainColorImage2Ds = global->swapchainColorImage2Ds;
		for (auto& swapchainColorImage2D : swapchainColorImage2Ds) {

			Image2DSystem::Destroy(context,
				swapchainColorImage2D);
		}

		vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
	}

}
