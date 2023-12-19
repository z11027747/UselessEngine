#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "base.h"

struct RenderGlobalComp final : public EngineComp {

	//VkInstance 底层就是个指针
	//#define VK_DEFINE_HANDLE(object)
	//	typedef struct object##_T* object;

	VkInstance instance;

	bool enabledDebug;
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
	bool needRecreateSwapchain;

	VkPipeline graphicsPipeline;
	std::vector<VkShaderModule> shaderModules;
	VkPipelineLayout graphicsPipelineLayout;
	VkRenderPass renderPass;

	std::vector<VkFramebuffer> swapchainFrameBuffers;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	uint32_t maxFrameInFlight;
	uint32_t currFrame;
	std::vector<VkFence> inFlightFences;

};