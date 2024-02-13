
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
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	void MaterialLightModelDescriptorLogic::CreateSetLayout(Context *context,
															std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding shadowMap = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding albedo = {
			1, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding normalMap = {
			2, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding materialUBO = {
			3, // binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(shadowMap);
		bindings.push_back(albedo);
		bindings.push_back(normalMap);
		bindings.push_back(materialUBO);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}

	constexpr int imageSize = 1 + 2; // shadow + albedo+normalMap
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

		auto &shadowPass = global->passMap[Define::Pass::Shadow];
		auto depthImageSampler = SamplerLogic::CreateDepth(context);

		VkDescriptorImageInfo shadowImageInfo = {
			depthImageSampler,
			shadowPass->depthImage2ds[0]->vkImageView,
			shadowPass->depthImage2ds[0]->layout};

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
								   [=](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   auto &bindings = graphicsPipeline->descriptorBindings;
									   for (auto imageIdx = 0; imageIdx < imageSize; imageIdx++)
									   {
										   DescriptorSetLogic::WriteImage(writes, descriptor->set, imageIdx,
																		  bindings[imageIdx].descriptorType, descriptor->imageInfos[imageIdx]);
									   }
									   auto bufferIdx = imageSize;
									   DescriptorSetLogic::WriteBuffer(writes, descriptor->set, bufferIdx,
																	   bindings[bufferIdx].descriptorType, descriptor->bufferInfos[0]);
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