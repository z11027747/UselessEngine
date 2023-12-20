#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <functional>

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
	static void MakeVertexInputCreateInfo(VkPipelineVertexInputStateCreateInfo&,
		VkVertexInputBindingDescription&, std::vector<VkVertexInputAttributeDescription>&);
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
	static void AllocateCommandBuffer(Context*, std::vector<VkCommandBuffer>&);
	static void FreeCommandBuffer(Context*, std::vector<VkCommandBuffer>&);
	static void RecordCommandBufferSingleTime(Context*, std::function<void(VkCommandBuffer&)>);

	static void AllocateSwapchainCommandBuffers(Context*);
	static void FreeSwapchainCommandBuffers(Context*);

	//draw
	static void DrawFrame(Context*);
	static void DrawRecord(Context*, uint32_t);

	static void WaitIdle(Context*);

	//semaphore
	static void CreateSemaphores(Context*);
	static void DestroySemaphores(Context*);

	//fence
	static void CreateFences(Context*);
	static void DestroyFences(Context*);

	//buffer
	static void CheckPhysicalDeviceMemory(Context*);
	static uint32_t FindMemoryType(Context*, uint32_t, VkMemoryPropertyFlags);
	static void CreateBuffer(Context*,
		VkDeviceSize, VkBufferUsageFlags,
		VkMemoryPropertyFlags,
		VkBuffer&, VkDeviceMemory&);
	static void DestroyBuffer(Context*,
		VkBuffer&, VkDeviceMemory&);
	static void CopyBuffer(Context*, VkBuffer&, VkBuffer&, VkDeviceSize);

	//vertexBuffer
	static void CreateVertexTriangle(Context*);
	static void CreateVertexBufferHost(Context*);
	static void CreateVertexBufferStageing(Context*);
	static void DestroyVertexBuffer(Context*);

	static void CreateVertexRectangle(Context*);
	static void CreateIndexBufferHost(Context*);
	static void DestroyIndexBuffer(Context*);

	//uniform
	static void CreateDescriptorSetLayout(Context*);
	static void DestroyDescriptorSetLayout(Context*);
	static void CreateUniformBuffersHost(Context*);
	static void DestroyUniformBuffers(Context*);
	static void UpdateUniformBuffer(Context*, uint32_t);
	static void CreateDescriptorPool(Context*);
	static void DestroyDescriptorPool(Context*);
	static void AllocateDescriptorSets(Context*);

	//texture
	static void CreateImage(Context*,
		uint32_t, uint32_t, VkFormat, VkImageTiling, VkImageUsageFlags,
		VkMemoryPropertyFlags,
		VkImage&, VkDeviceMemory&);
	static void DestroyImage(Context*, VkImage&, VkDeviceMemory&);
	static void TransitionImageLayout(Context*, VkImage&, VkImageLayout, VkImageLayout);
	static void CopyBufferToImage(Context*, VkBuffer&, VkImage&, uint32_t, uint32_t);

	static void CreateImageView(Context*, VkFormat, VkImage&, VkImageView&);
	static void DestroyImageView(Context*, VkImageView&);

	static void CreateTextureImage(Context*);
	static void DestroyTextureImage(Context*);
	static void CreateTextureImageView(Context*);
	static void DestroyTextureImageView(Context*);
	static void CreateTextureSampler(Context*);
	static void DestroyTextureSampler(Context*);
};