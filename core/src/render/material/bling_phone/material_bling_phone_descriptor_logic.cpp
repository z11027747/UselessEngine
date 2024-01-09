
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_bling_phone_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialBlingPhoneDescriptorLogic::CreateSetLayout(Context *context,
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
	void MaterialBlingPhoneDescriptorLogic::DestroySetLayout(Context *context,
															 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
	}
	void MaterialBlingPhoneDescriptorLogic::AllocateAndUpdate(Context *context,
															  std::shared_ptr<Material> material)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &graphicsPipeline = global->pipelines[material->pipelineName];
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);

		auto sampler0 = SamplerLogic::Create(context);
		auto samplerShadow1 = SamplerLogic::Create(context, true);

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->set = descriptorSet;

		descriptor->image0Info = {
			sampler0,
			material->image0->vkImageView,
			material->image0->layout};

		auto &shadowPass = global->passes[Pass_Shadow];
		auto depthImage2d = shadowPass->depthImage2ds[0];

		descriptor->image1Info = {
			samplerShadow1,
			depthImage2d->vkImageView,
			depthImage2d->layout};

		material->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
								   [&](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   DescriptorSetLogic::WriteImage(context,
																	  writes,
																	  material->descriptor);
								   });
	}
	void MaterialBlingPhoneDescriptorLogic::Destroy(Context *context,
													std::shared_ptr<Material> material)
	{
		auto &descriptor = material->descriptor;
		SamplerLogic::Destroy(context, descriptor->image0Info.sampler);
		SamplerLogic::Destroy(context, descriptor->image1Info.sampler);
	}
}