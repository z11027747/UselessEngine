#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/image/image_comp.h"

namespace Render {

	struct Uniform;

	struct GraphicsPipeline final {
		std::string name;

		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;

		std::vector<VkShaderModule> shaderModules;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfo;
		std::vector<VkVertexInputAttributeDescription> vertexAttrDescriptions;

		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<std::shared_ptr<Uniform>> uniforms;
	};

	struct Uniform {
		virtual ~Uniform() {}  
		VkDescriptorSet descriptorSet;
	};

	struct UniformTest final : public Uniform {
		std::shared_ptr<Buffer> uniform0Buffer;
		std::shared_ptr<Buffer> uniform1Buffer;
		VkSampler sampler;
	};

	struct CameraUBO final {
		glm::mat4 view;
		glm::mat4 proj;
	};

	struct TransformUBO final {
		glm::mat4 model;
	};

	struct Texture2D final {
		int width;
		int height;
		unsigned char* data;
	};


}