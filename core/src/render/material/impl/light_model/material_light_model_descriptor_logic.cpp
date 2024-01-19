
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_light_model_logic.h"
#include "render/render_pass/render_pass_comp.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialLightModelDescriptorLogic::CreateSetLayout(Context *context,
															std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding samplerBinding0 = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding samplerBinding1 = {
			1, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding samplerBinding2 = {
			2, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding samplerBinding3 = {
			3, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(samplerBinding0);
		bindings.push_back(samplerBinding1);
		bindings.push_back(samplerBinding2);
		bindings.push_back(samplerBinding3);

		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}
	void MaterialLightModelDescriptorLogic::DestroySetLayout(Context *context,
															 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
	}
	void MaterialLightModelDescriptorLogic::AllocateAndUpdate(Context *context,
															  std::shared_ptr<MaterialInstance> instance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &graphicsPipeline = global->pipelines[instance->pipelineName];
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
		descriptor->set = descriptorSet;

		auto &shadowPass = global->passes[Pass_Shadow];
		auto depthImageSampler = SamplerLogic::Create(context, true, true);

		VkDescriptorImageInfo shadowImageInfo = {
			depthImageSampler,
			shadowPass->depthImage2ds[0]->vkImageView,
			shadowPass->depthImage2ds[0]->layout};

		descriptor->imageInfos.push_back(shadowImageInfo);

		static int imageSize = 1+3; // shadow + albedo+specular+normalMap

		for (auto i = 0; i < imageSize - 1; i++)
		{
			auto &image = instance->images[i];
			VkDescriptorImageInfo imageInfo = {
				global->globalSamplerRepeat,
				instance->images[i]->vkImageView,
				instance->images[i]->layout};
			descriptor->imageInfos.push_back(imageInfo);
		}

		instance->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
								   [&descriptor](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   for (auto i = 0; i < imageSize; i++)
									   {
										   DescriptorSetLogic::WriteImage(writes,
																		  descriptor->set, i, descriptor->imageInfos[i]);
									   }
								   });
	}
	void MaterialLightModelDescriptorLogic::Destroy(Context *context,
													std::shared_ptr<MaterialInstance> instance)
	{
		auto &descriptor = instance->descriptor;
		SamplerLogic::Destroy(context, descriptor->imageInfos[0].sampler);
	}
}