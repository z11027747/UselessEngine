#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "render/vk/image/image_comp.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"

namespace Render
{
	struct Global final
	{
		inline static std::string type{"Render::Global"};

		VkInstance instance;

		bool enabledDebug{true};
		VkDebugUtilsMessengerEXT debugUtilsMessenger;

		VkPhysicalDevice physicalDevice;
		uint32_t physicalQueueFamilyIndex;
		VkDevice logicalDevice;
		VkQueue logicalQueue;

		VkSurfaceKHR surface;
		VkSurfaceFormatKHR surfaceFormat;
		VkPresentModeKHR surfacePresentMode;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		VkFormat depthFormat;
		VkFormat depthStencilFormat;
		VkSampleCountFlagBits msaaSamples{VK_SAMPLE_COUNT_1_BIT};

		VkSwapchainKHR swapchain;
		uint32_t swapchainImageCount;
		std::vector<std::shared_ptr<Image>> swapchainImages;
		std::vector<VkCommandBuffer> swapchainCmdBuffers;

		const uint32_t maxConcurrentFrame = 2;

		std::vector<uint32_t> swapchainImageIndexs;

		uint32_t currFrame;
		std::vector<VkFence> waitFences;
		std::vector<VkSemaphore> presentCompleteSemaphores;
		std::vector<VkSemaphore> renderCompleteSemaphores;

		VkCommandPool vkPool;
		VkDescriptorPool descriptorPool;

		std::unordered_map<std::string, std::shared_ptr<Render::Pass>> passMap;
		std::unordered_map<std::string, std::shared_ptr<Render::GraphicsPipeline>> pipelineMap;

		std::vector<VkCommandBuffer> batchCmdBuffers;
		std::vector<VkCommandBuffer> tempCmdBuffers;

		std::vector<std::shared_ptr<Render::Buffer>> tempBuffers;

		VkDescriptorSetLayout globalDescriptorSetLayout;
		std::shared_ptr<Descriptor> globalDescriptor;
		std::shared_ptr<Buffer> globalBuffer;

		VkSampler globalSamplerRepeat;
		VkSampler globalSamplerClampLinear;
		VkSampler globalSamplerClampNearest;
	};
}