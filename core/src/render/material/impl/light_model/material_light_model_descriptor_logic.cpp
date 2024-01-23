
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
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
		VkDescriptorSetLayoutBinding shadowSampler = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding albedoSampler = {
			1, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding specularSampler = {
			2, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding normalMapSampler = {
			3, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding lightModelUBO = {
			4, // binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(shadowSampler);
		bindings.push_back(albedoSampler);
		bindings.push_back(specularSampler);
		bindings.push_back(normalMapSampler);
		bindings.push_back(lightModelUBO);

		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}
	void MaterialLightModelDescriptorLogic::DestroySetLayout(Context *context,
															 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;
		DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
	}

	static int imageSize = 1 + 3; // shadow + albedo+specular+normalMap

	void MaterialLightModelDescriptorLogic::AllocateAndUpdate(Context *context,
															  std::shared_ptr<MaterialInstance> instance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &info = instance->info;
		auto &graphicsPipeline = global->pipelineMap[info->pipelineName];
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
		descriptor->set = descriptorSet;

		auto &shadowPass = global->passMap[Pass_Shadow];
		auto depthImageSampler = SamplerLogic::CreateDepth(context);

		VkDescriptorImageInfo shadowImageInfo = {
			depthImageSampler,
			shadowPass->depthImage2d->vkImageView,
			shadowPass->depthImage2d->layout};

		descriptor->imageInfos.push_back(shadowImageInfo);

		for (auto i = 0; i < imageSize - 1; i++)
		{
			auto &image = instance->images[i];
			auto imageSamplerWithMipMap = SamplerLogic::Create(context, false,
															   0, image->mipLevels);

			VkDescriptorImageInfo imageInfo = {
				imageSamplerWithMipMap,
				instance->images[i]->vkImageView,
				instance->images[i]->layout};
			descriptor->imageInfos.push_back(imageInfo);
		}

		// buffer
		VkDescriptorBufferInfo bufferInfo = {
			instance->buffer->vkBuffer,
			0,
			instance->buffer->size};
		descriptor->bufferInfos.push_back(bufferInfo);

		instance->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
								   [&descriptor](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   for (auto i = 0; i < imageSize; i++)
									   {
										   DescriptorSetLogic::WriteImage(writes,
																		  descriptor->set, i, descriptor->imageInfos[i]);
									   }

									   DescriptorSetLogic::WriteBuffer(writes,
																	   descriptor->set, imageSize, descriptor->bufferInfos[0]);
								   });
	}
	void MaterialLightModelDescriptorLogic::Destroy(Context *context,
													std::shared_ptr<MaterialInstance> instance)
	{
		auto &descriptor = instance->descriptor;
		for (auto i = 0; i < imageSize; i++)
		{
			SamplerLogic::Destroy(context, descriptor->imageInfos[i].sampler);
		}
	}
}