
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/shader_skybox_logic.h"
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

	void ShaderSkyboxLogic::CreateVertexAttrDescriptions(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = 0;

		graphicsPipeline->vertexAttrDescriptions = {
			positionOSDescription0
		};
	}

	void ShaderSkyboxLogic::MakeDepthStencilCreateInfo(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline,
		VkPipelineDepthStencilStateCreateInfo& depthStencilStateCreateInfo
	) {
		depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilStateCreateInfo.depthTestEnable = false;
		depthStencilStateCreateInfo.depthWriteEnable = false;
		depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilStateCreateInfo.depthBoundsTestEnable = false;
		depthStencilStateCreateInfo.stencilTestEnable = false;
	}

	void ShaderSkyboxLogic::MakeRasterizationCreateInfo(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline,
		VkPipelineRasterizationStateCreateInfo& rasterizationStateCreateInfo
	) {
		rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateCreateInfo.rasterizerDiscardEnable = false;
		rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationStateCreateInfo.lineWidth = 1.0f;
		rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
		rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationStateCreateInfo.depthClampEnable = false;
		rasterizationStateCreateInfo.depthBiasEnable = false;
	}

	void ShaderSkyboxLogic::CreateDescriptorSetLayout(Context* context,
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

	void ShaderSkyboxLogic::DestroyDescriptorSetLayout(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
	}

	void ShaderSkyboxLogic::UpdateDescriptorSets(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
	}

	void ShaderSkyboxLogic::CreateUnitDescriptor(Context* context,
		std::shared_ptr<Unit> unit,
		std::shared_ptr<Image> imageCube
	) {
		auto& graphicsPipeline = context->renderPipelines[unit->pipelineName];
		auto& descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
		auto sampler = SamplerLogic::Create(context);

		VkDescriptorImageInfo imageInfo = {
				sampler,
				imageCube->vkImageView,
				imageCube->layout
		};

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->set = descriptorSet;
		descriptor->image = imageCube;
		descriptor->imageInfo = imageInfo;

		unit->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
			[&](std::vector<VkWriteDescriptorSet>& writes) {

				DescriptorSetLogic::WriteImage(context,
				writes,
				unit->descriptor);
			});
	}

	void ShaderSkyboxLogic::DestroyUnitDescriptor(Context* context,
		std::shared_ptr<Unit> unit
	) {
		auto& descriptor = unit->descriptor;
		ImageLogic::Destroy(context, descriptor->image);
		SamplerLogic::Destroy(context, descriptor->imageInfo.sampler);
	}

	void ShaderSkyboxLogic::UpdateUnitDescriptor(Context* context,
		std::shared_ptr<Unit> unit
	) {
	}

}