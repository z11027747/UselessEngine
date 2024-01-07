
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/shader_impl/shader_bling_phone_logic.h"
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

namespace Render
{
	void ShaderBlingPhoneLogic::CreateVertexAttrDescriptions(Context *context,
															 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkVertexInputAttributeDescription positionOSDescription0 = {};
		positionOSDescription0.location = 0;
		positionOSDescription0.binding = 0;
		positionOSDescription0.format = VK_FORMAT_R32G32B32_SFLOAT;
		positionOSDescription0.offset = offsetof(Render::Vertex, positionOS);

		VkVertexInputAttributeDescription normalOSDescription1 = {};
		normalOSDescription1.location = 1;
		normalOSDescription1.binding = 0;
		normalOSDescription1.format = VK_FORMAT_R32G32B32_SFLOAT;
		normalOSDescription1.offset = offsetof(Render::Vertex, normalOS);

		VkVertexInputAttributeDescription colorDescription2 = {};
		colorDescription2.location = 2;
		colorDescription2.binding = 0;
		colorDescription2.format = VK_FORMAT_R32G32B32_SFLOAT;
		colorDescription2.offset = offsetof(Render::Vertex, color);

		VkVertexInputAttributeDescription uv0Description3 = {};
		uv0Description3.location = 3;
		uv0Description3.binding = 0;
		uv0Description3.format = VK_FORMAT_R32G32_SFLOAT;
		uv0Description3.offset = offsetof(Render::Vertex, uv0);

		auto &stageInfo = graphicsPipeline->stageInfo;
		stageInfo.vertexInputAttributeDescriptions = {
			positionOSDescription0,
			normalOSDescription1,
			colorDescription2,
			uv0Description3};
	}

	void ShaderBlingPhoneLogic::SetRasterizationCreateInfo(Context *context,
														   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
		// rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	}

	void ShaderBlingPhoneLogic::CreateDescriptorSetLayout(Context *context,
														  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding samplerBinding0 = {};
		samplerBinding0.binding = 0;
		samplerBinding0.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerBinding0.descriptorCount = 1;
		samplerBinding0.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutBinding samplerBinding1 = {};
		samplerBinding1.binding = 1;
		samplerBinding1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerBinding1.descriptorCount = 1;
		samplerBinding1.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(samplerBinding0);
		bindings.push_back(samplerBinding1);

		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}

	void ShaderBlingPhoneLogic::DestroyDescriptorSetLayout(Context *context,
														   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
	}

	void ShaderBlingPhoneLogic::CreateUnitDescriptor(Context *context,
													 std::shared_ptr<Unit> unit)
	{
		auto &graphicsPipeline = context->renderPipelines[unit->pipelineName];
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);

		auto sampler0 = SamplerLogic::Create(context);
		auto samplerShadow1 = SamplerLogic::Create(context, true);

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->set = descriptorSet;

		VkDescriptorImageInfo vkImage0Info = {
			sampler0,
			unit->image0->vkImageView,
			unit->image0->layout};
		descriptor->vkImage0Info = vkImage0Info;

		auto &renderShadowPass = context->renderShadowPass;
		auto depthImage2d = renderShadowPass->depthImage2ds[0];

		VkDescriptorImageInfo vkImage1Info = {
			samplerShadow1,
			depthImage2d->vkImageView,
			depthImage2d->layout};
		descriptor->vkImage1Info = vkImage1Info;

		unit->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
								   [&](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   DescriptorSetLogic::WriteImage(context,
																	  writes,
																	  unit->descriptor);
								   });
	}

	void ShaderBlingPhoneLogic::DestroyUnitDescriptor(Context *context,
													  std::shared_ptr<Unit> unit)
	{
		auto &descriptor = unit->descriptor;
		ImageLogic::Destroy(context, unit->image0);
		SamplerLogic::Destroy(context, descriptor->vkImage0Info.sampler);
		SamplerLogic::Destroy(context, descriptor->vkImage1Info.sampler);
	}

	void ShaderBlingPhoneLogic::UpdateUnitDescriptor(Context *context,
													 std::shared_ptr<Unit> unit,
													 uint32_t imageIndex)
	{
	}

}