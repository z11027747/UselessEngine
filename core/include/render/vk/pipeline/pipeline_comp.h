#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <memory>
#include "render/vk/pipeline/descriptor_comp.h"

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

	struct GraphicsPipeline final
	{
		std::string name;
		PipelineStageInfo stageInfo;
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
		VkDescriptorSetLayout descriptorSetLayout;
	};
}