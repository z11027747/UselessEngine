#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class Context;

class RenderSystem final {
public:

	static void CreateGlobal(Context*);

	//instance
	static void CreateInstance(Context*);
	static void DestroyInstance(Context*);
	static bool CheckInstanceExtension(const char*);
	static bool CheckInstanceLayer(const char*);

	//debugCallback
	static void CreateDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
	static void CreateDebugCallback(Context*);
	static void DestroyDebugCallback(Context*);

	//physicsDevice
	static void PickupPhysicalDevice(Context*);

	//logicDevice
	static void CreateLogicDevice(Context*);
	static void GetLogicDeviceQueue(Context*);
	static void DestroyLogicDevice(Context*);

	//surface
	static void CreateSurface(Context*);
	static void DestroySurface(Context*);

	//swapchain
	static bool CheckSwapchainSupport(Context*, const VkPhysicalDevice&, int);
	static VkSurfaceFormatKHR GetSwapchainSurfaceFormat(Context*);
	static VkPresentModeKHR GetSwapchainPresentMode(Context*);
	static VkSurfaceCapabilitiesKHR GetSwapchainCapbilities(Context*);
	static void CreateSwapchian(Context*);
	static void DestroySwapchian(Context*);
	static void GetSwapchianImages(Context*);
	static void CreateSwapchianImageViews(Context*);
	static void DestroySwapchianImageViews(Context*);

	//graphicsPipeline

	//shader
	static void CreateShader(Context*, const std::string&);
	static void DestroyAllShaders(Context*);

	//memory
	static void CheckPhysicalDeviceMemory(Context*);
};