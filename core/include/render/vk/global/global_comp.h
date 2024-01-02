#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "render/vk/image/image_comp.h"
#include "render/vk/cmd/cmd_comp.h"

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

		VkSwapchainKHR swapchain;
		uint32_t swapchainImageCount;
		std::vector<std::shared_ptr<Image>> swapchainImages;
		std::shared_ptr<Cmd> swapchainCmd;

		uint32_t currFrame;
		uint32_t maxFrameInFlight;
		std::vector<VkFence> inFlightFences;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

		VkCommandPool vkPool;
		VkDescriptorPool descriptorPool;
	};

}