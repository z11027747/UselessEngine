#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "base.h"

struct RenderVertex final {
	glm::vec2 pos;
	glm::vec3 color;

	//顶点数据布局信息
	static VkVertexInputBindingDescription CreateBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};

		//binding index
		bindingDescription.binding = 0;
		//指定顶点数据的输入率，顶点or实例
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		//指定每个顶点数据的字节数
		bindingDescription.stride = sizeof(RenderVertex);

		return bindingDescription;
	}

	//顶点属性的格式和位置
	static std::vector<VkVertexInputAttributeDescription> CreateAttributeDescriptions() {

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

		//location index
		attributeDescriptions[0].location = 0;
		//binding index
		attributeDescriptions[0].binding = 0;
		//数据格式, float float
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		//属性在每个顶点数据中的偏移量。
		attributeDescriptions[0].offset = offsetof(RenderVertex, pos);

		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(RenderVertex, color);

		return attributeDescriptions;
	}
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
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	uint32_t maxFrameInFlight;
	uint32_t currFrame;
	std::vector<VkFence> inFlightFences;

	std::vector<RenderVertex> vertices;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

};
