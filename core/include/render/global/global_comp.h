#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace Render {

	struct Image2D;

	struct Global final {

		VkInstance instance;

		VkPhysicalDevice physicalDevice;
		uint32_t physicalQueueFamilyIndex;

		VkDevice logicalDevice;
		VkQueue logicalQueue;

		VkSurfaceKHR surface;
		VkSurfaceFormatKHR surfaceFormat;
		VkPresentModeKHR surfacePresentMode;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;

		VkRenderPass renderPass;

		VkSwapchainKHR swapchain;
		std::vector<std::shared_ptr<Image2D>> swapchainColorImage2Ds;
		std::vector<std::shared_ptr<Image2D>> swapchainDepthImage2Ds;
		VkFormat depthImageFormat;

		std::vector<VkFramebuffer> frameBuffers;
		std::vector<VkCommandBuffer> cmdBuffers;

		uint32_t currFrame;
		uint32_t maxFrameInFlight;
		std::vector<VkFence> inFlightFences;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

	};

}