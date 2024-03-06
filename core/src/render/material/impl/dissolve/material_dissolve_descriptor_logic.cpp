
#include <vulkan/vulkan.h>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/material/impl/material_dissolve_logic.h"
#include "context.hpp"

namespace Render
{
	void MaterialDissolveDescriptorLogic::CreateSetLayout(Context *context,
														  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding albedo = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding dissolve = {
			1, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding ramp = {
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
		bindings.push_back(albedo);
		bindings.push_back(dissolve);
		bindings.push_back(ramp);
		bindings.push_back(materialUBO);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}

	constexpr int imageSize = 3; // albedo+dissolve+ramp
	void MaterialDissolveDescriptorLogic::AllocateAndUpdate(Context *context,
															std::shared_ptr<MaterialData> data)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &info = data->info;
		auto &graphicsPipeline = global->pipelineMap[info->pipelineName];
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
		descriptor->set = descriptorSet;

		for (auto i = 0; i < imageSize; i++)
		{
			auto &image = data->images[i];
			auto imageSamplerWithMipMap = SamplerLogic::Create(context, true, false,
															   0, image->mipLevels);

			VkDescriptorImageInfo imageInfo = {
				imageSamplerWithMipMap,
				data->images[i]->vkImageView,
				data->images[i]->layout};
			descriptor->imageInfos.push_back(imageInfo);
		}

		VkDescriptorBufferInfo bufferInfo = {
			data->buffer->vkBuffer,
			0,
			data->buffer->size};
		descriptor->bufferInfos.push_back(bufferInfo);

		data->descriptor = descriptor;

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
	void MaterialDissolveDescriptorLogic::Destroy(Context *context,
												  std::shared_ptr<MaterialData> data)
	{
		auto &descriptor = data->descriptor;
		for (auto i = 0; i < imageSize; i++)
		{
			SamplerLogic::Destroy(context, descriptor->imageInfos[i].sampler);
		}
	}
}