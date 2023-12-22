#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace Render {

	struct Pipeline final {

		VkPipeline pipeline;
		std::vector<VkShaderModule> shaderModules;
		VkPipelineLayout pipelineLayout;

		VkDescriptorSetLayout descriptorSetLayout;
	};

	struct DescriptorPool final {

		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
	};

}