
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

	void ShaderTestLogic::UpdateDescriptorSets(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderTestLogic::CreateUnitDescriptor(Context* context,
		std::shared_ptr<Unit> unit,
		std::shared_ptr<Image> image
	) {
		auto& graphicsPipeline = context->renderPipelines[unit->pipelineName];
		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
		auto sampler = SamplerLogic::Create(context, 0);

		VkDescriptorImageInfo imageInfo = {
				sampler,
				image->vkImageView,
				image->layout
		};

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->set = descriptorSet;
		descriptor->image = image;
		descriptor->imageInfo = imageInfo;

		unit->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
			[&](std::vector<VkWriteDescriptorSet>& writes) {

				DescriptorSetLogic::WriteImage(context,
				writes,
				unit->descriptor);

			});
	}

	void ShaderTestLogic::DestroyUnitDescriptor(Context* context,
		std::shared_ptr<Unit> unit
	) {
		auto& descriptor = unit->descriptor;
		ImageLogic::Destroy(context, descriptor->image);
		SamplerLogic::Destroy(context, descriptor->imageInfo.sampler);
	}

	void ShaderTestLogic::UpdateUnitDescriptor(Context* context,
		std::shared_ptr<Unit> unit
	) {
	}

}