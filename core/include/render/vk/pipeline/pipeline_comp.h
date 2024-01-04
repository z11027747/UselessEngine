#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"

namespace Render
{
	struct PipelineStageInfo final
	{
		std::vector<VkShaderModule> shaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;

		std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
		VkVertexInputBindingDescription vertexBindingDescription;
		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportStateCreateInfo;

		VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;

		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;

		VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;

		VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
		VkPipelineColorBlendStateCreateInfo colorBlendingStateCreateInfo;
	};

	struct Descriptor;

	struct GraphicsPipeline final
	{
		std::string name;

		PipelineStageInfo stageInfo;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;

		VkDescriptorSetLayout globalDescriptorSetLayout;
		std::vector<std::shared_ptr<Descriptor>> globalDescriptors;
		std::vector<std::shared_ptr<Buffer>> globalBuffers;

		VkDescriptorSetLayout descriptorSetLayout;
	};

	struct Descriptor final
	{
		VkDescriptorSet set;
		VkDescriptorBufferInfo bufferInfo;
		VkDescriptorImageInfo vkImage0Info;
		VkDescriptorImageInfo vkImage1Info;
	};

	struct CameraUBO final
	{
		alignas(16) glm::vec3 pos;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
	};

	struct DirectionLightUBO final
	{
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 projection;
		alignas(16) glm::vec3 dir;
		alignas(16) glm::vec3 col;
		alignas(16) glm::vec4 params;
	};

	struct GlobalUBO final
	{
		alignas(16) CameraUBO camera;
		alignas(16) DirectionLightUBO directionLight;
	};

}