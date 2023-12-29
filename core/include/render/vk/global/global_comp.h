#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "render/vk/image/image_comp.h"

namespace Render {

	struct Global final {

		VkInstance instance;

		bool enabledDebug;
		VkDebugUtilsMessengerEXT debugUtilsMessenger;

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
		std::vector<std::shared_ptr<Image>> swapchainColorImage2Ds;
		std::vector<std::shared_ptr<Image>> swapchainDepthImage2Ds;
		VkFormat depthImageFormat;

		std::vector<VkFramebuffer> frameBuffers;
		std::vector<VkCommandBuffer> cmdBuffers;

		uint32_t currFrame;
		uint32_t maxFrameInFlight;
		std::vector<VkFence> inFlightFences;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

		VkDescriptorPool descriptorPool;
	};

}