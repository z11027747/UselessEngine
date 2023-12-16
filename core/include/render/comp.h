#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "base.h"

struct RenderGlobal final : public EngineComp {

	//VkInstance 底层就是个指针
	//#define VK_DEFINE_HANDLE(object)
	//	typedef struct object##_T* object;

	VkInstance instance;

	bool enableValidationLayer;
	VkDebugUtilsMessengerEXT debugCallback;

	VkPhysicalDevice physicalDevice;
	int physicalDeviceGraphicsFamily;

	VkDevice logicDevice;
	VkQueue logicQueue;

	VkSurfaceKHR surface;

	VkSwapchainKHR swapchain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	
};