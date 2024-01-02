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

		VkDescriptorSetLayout descriptorSetLayout;
	};

	struct Descriptor final
	{
		VkDescriptorSet set;

		std::shared_ptr<Buffer> buffer;
		VkDescriptorBufferInfo bufferInfo;

		std::shared_ptr<Image> image;
		VkDescriptorImageInfo imageInfo;
	};

	struct GlobalUBO final
	{
		// camera
		alignas(16) glm::vec3 cameraPos;		// 12+4
		alignas(16) glm::mat4 cameraView;		// 64
		alignas(16) glm::mat4 cameraProjection; // 64

		// directionLight
		alignas(16) glm::vec3 directionLightPos; // 12+4
		alignas(16) glm::vec3 directionLightCol; // 12+4
		//x:diffuseIntensity y:specualrShininess z:specularIntensity
		alignas(16) glm::vec4 directionLightParams; // 12+4
	};

}