#pragma once

#include <vulkan/vulkan.h>
#include <vector>


namespace Render {

	struct Swapchain final {

		VkSurfaceKHR surface;
		VkSurfaceFormatKHR surfaceFormat;
		VkPresentModeKHR surfacePresentMode;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;

		VkSwapchainKHR vkSwapchain;

		VkFormat format;
		VkExtent2D extent;
		std::vector<VkImage> images;
		std::vector<VkImageView> imageViews;

		std::vector<VkFramebuffer> frameBuffers;
		std::vector<VkCommandBuffer> cmdBuffers;

		uint32_t currFrame;
		uint32_t maxFrameInFlight;
		std::vector<VkFence> inFlightFences;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

	};

}