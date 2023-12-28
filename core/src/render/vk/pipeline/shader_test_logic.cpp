
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/shader_test_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	void ShaderTestLogic::CreateVertexAttrDescriptions(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = offsetof(Render::Vertex, positionOS);

		VkVertexInputAttributeDescription colorDescription1 = {};
		colorDescription1.location = 1;
		colorDescription1.binding = 0;
		colorDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
		colorDescription1.offset = offsetof(Render::Vertex, color);

		VkVertexInputAttributeDescription uv0Description2 = {};
		uv0Description2.location = 2;
		uv0Description2.binding = 0;
		uv0Description2.format = VK_FORMAT_R32G32_SFLOAT;
		uv0Description2.offset = offsetof(Render::Vertex, uv0);

		graphicsPipeline->vertexAttrDescriptions = {
			positionOSDescription0,
			colorDescription1,
			uv0Description2,
		};
	}

	void ShaderTestLogic::CreateDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkDescriptorSetLayoutBinding samplerBinding0 = {};
		samplerBinding0.binding = 0;
		samplerBinding0.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerBinding0.descriptorCount = 1;
		samplerBinding0.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(samplerBinding0);

		graphicsPipeline->descriptorSetLayout
			= DescriptorSetLayoutLogic::Create(context, bindings);
	}

	void ShaderTestLogic::DestroyDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
	}

	void ShaderTestLogic::CreateUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		auto descriptorSets = DescriptorSetLogic::Allocate(context, descriptorSetLayout);

		for (auto i = 0; i < descriptorSets.size(); i++) {
			auto uniformTest = std::make_shared<UniformTest>();

			uniformTest->descriptorSet = descriptorSets[i];
			uniformTest->sampler = SamplerLogic::Create(context, 0);

			graphicsPipeline->uniforms.push_back(uniformTest);
		}
	}

	void ShaderTestLogic::DestroyUniforms(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& uniforms = graphicsPipeline->uniforms;
		for (auto& uniform : uniforms) {
			auto uniformTest = std::dynamic_pointer_cast<UniformTest>(uniform);

			SamplerLogic::Destroy(context,
				uniformTest->sampler);
		}
		uniforms.clear();
	}

	void ShaderTestLogic::UpdateDescriptorSets(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderTestLogic::UpdateUniformBuffer(Context* context,
		std::shared_ptr<EngineObject> unitEO,
		std::shared_ptr<Uniform> uniform
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto uniformTest = std::dynamic_pointer_cast<UniformTest>(uniform);

		auto unit = unitEO->GetComponent<Render::Unit>();
		auto& image2D = unit->image2D;

		VkWriteDescriptorSet samplerWrite2 = {};
		samplerWrite2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		samplerWrite2.dstSet = uniformTest->descriptorSet;
		samplerWrite2.dstBinding = 0;
		samplerWrite2.dstArrayElement = 0;
		samplerWrite2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerWrite2.descriptorCount = 1;

		VkDescriptorImageInfo samplerImage2 = {};
		samplerImage2.sampler = uniformTest->sampler;
		samplerImage2.imageView = image2D->vkImageView;
		samplerImage2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		samplerWrite2.pImageInfo = &samplerImage2;

		vkUpdateDescriptorSets(logicalDevice, 1, &samplerWrite2, 0, nullptr);
	}

}