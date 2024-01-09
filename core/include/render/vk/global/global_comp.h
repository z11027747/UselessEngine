#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "render/vk/image/image_comp.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"

namespace Render
{
	const std::string Pass_ImGui = "imGui";
	const std::string Pass_Shadow = "Shadow";
	const std::string Pass_Main = "main";

	const std::string Pipeline_Skybox = "skybox";
	const std::string Pipeline_Shadow = "shadow";
	const std::string Pipeline_Bling_Phone = "bling_phone";
	const std::string Pipeline_Color = "color";

	struct Global final
	{
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
		VkFormat depthFormat;

		VkSwapchainKHR swapchain;
		uint32_t swapchainImageCount;
		std::vector<std::shared_ptr<Image>> swapchainImages;
		std::vector<VkCommandBuffer> swapchainCmdBuffers;

		uint32_t currFrame;
		uint32_t maxFrameInFlight;
		std::vector<VkFence> inFlightFences;
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;

		VkCommandPool vkPool;
		VkDescriptorPool descriptorPool;

		std::unordered_map<std::string, std::shared_ptr<Render::Pass>> passes;
		std::unordered_map<std::string, std::shared_ptr<Render::GraphicsPipeline>> pipelines;

		std::vector<VkCommandBuffer> batchCmdBuffers;
		std::vector<VkCommandBuffer> tempCmdBuffers;

		std::vector<std::shared_ptr<Render::Buffer>> tempBuffers;
		
		VkDescriptorSetLayout globalDescriptorSetLayout;
		std::shared_ptr<Descriptor> globalDescriptor;
		std::shared_ptr<Buffer> globalBuffer;
	};
}