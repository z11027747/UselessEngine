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
	static void MakeDebugCreateInfo(VkDebugUtilsMessengerCreateInfoEXT&);
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
	static void SetNeedRecreateSwapchain(Context*);
	static void TryRecreateSwapchain(Context*);

	//shader
	static void CreateShader(Context*, const std::string&);
	static void DestroyAllShaders(Context*);
	static void MakeShaderModuleCreateInfos(std::vector<VkShaderModule>&, std::vector<VkPipelineShaderStageCreateInfo>&);

	//fixed
	static void MakeVertexInputCreateInfo(VkPipelineVertexInputStateCreateInfo&);
	static void MakeInputAssemblyCreateInfo(VkPipelineInputAssemblyStateCreateInfo&);
	static void MakeViewportCreateInfo(VkViewport&, VkRect2D&, VkPipelineViewportStateCreateInfo&, VkExtent2D&);
	static void MakeRasterizationCreateInfo(VkPipelineRasterizationStateCreateInfo&);
	static void MakeMultisampleCreateInfo(VkPipelineMultisampleStateCreateInfo&);
	static void MakeColorBlendCreateInfo(VkPipelineColorBlendAttachmentState&, VkPipelineColorBlendStateCreateInfo&);

	//layout
	static void CreatePipelineLayout(Context*);
	static void DestroyPipelineLayout(Context*);

	//renderPass
	static void CreateRenderPass(Context*);
	static void DestroyRenderPass(Context*);

	//graphicsPipeline
	static void CreateGraphicsPipeline(Context*);
	static void DestroyGraphicsPipeline(Context*);

	//frameBuffer
	static void CreateFrameBuffers(Context*);
	static void DestroyFrameBuffers(Context*);

	//command
	static void CreateCommandPool(Context*);
	static void DestroyCommandPool(Context*);
	static void CreateCommandBuffers(Context*);
	static void FreeCommandBuffers(Context*);
	static void RecordCommandBuffer(Context*, uint32_t);

	//draw
	static void DrawFrame(Context*);
	static void DrawWait(Context*);

	//semaphore
	static void CreateSemaphores(Context*);
	static void DestroySemaphores(Context*);

	//fence
	static void CreateFences(Context*);
	static void DestroyFences(Context*);

	//memory
	static void CheckPhysicalDeviceMemory(Context*);
};