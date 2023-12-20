#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "base.h"

struct RenderVertex final {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription CreateBindingDescription();
	static std::vector<VkVertexInputAttributeDescription> CreateAttributeDescriptions();
};

struct UniformBufferObject final {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

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

	std::vector<VkCommandBuffer> swapchainCommandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	uint32_t maxFrameInFlight;
	uint32_t currFrame;
	std::vector<VkFence> inFlightFences;

	std::vector<RenderVertex> vertices;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	std::vector<uint16_t> indices;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBufferMemorys;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
};
