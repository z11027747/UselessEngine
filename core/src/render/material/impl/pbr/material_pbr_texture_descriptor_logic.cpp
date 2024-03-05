
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_pbr_logic.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	void MaterialPBRTextureDescriptorLogic::CreateSetLayout(Context *context,
															std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding albedoMap = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding normalMap = {
			1, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding roughnessMap = {
			2, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding metallicMap = {
			3, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding BRDFLUTMap = {
			4, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding skyboxCubeMap = {
			5, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding materialUBO = {
			6, // binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(albedoMap);
		bindings.push_back(normalMap);
		bindings.push_back(roughnessMap);
		bindings.push_back(metallicMap);
		bindings.push_back(BRDFLUTMap);
		bindings.push_back(skyboxCubeMap);
		bindings.push_back(materialUBO);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}

	constexpr int imageCount = 5 + 1;
	void MaterialPBRTextureDescriptorLogic::AllocateAndUpdate(Context *context,
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

		for (auto i = 0; i < imageCount - 1; i++)
		{
			VkDescriptorImageInfo imageInfo = {
				global->globalSamplerClampLinear,
				data->images[i]->vkImageView,
				data->images[i]->layout};
			descriptor->imageInfos.push_back(imageInfo);
		}
		{
			auto skyboxEO = context->GetEO(Define::EOName::Skybox);
			auto &skyboxData = skyboxEO->GetComponent<Material>()->data;

			VkDescriptorImageInfo imageInfo = {
				global->globalSamplerClampLinear,
				skyboxData->images[0]->vkImageView,
				skyboxData->images[0]->layout};
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
									   for (auto i = 0; i < imageCount; i++)
									   {
										   DescriptorSetLogic::WriteImage(writes, descriptor->set, i,
																		  bindings[i].descriptorType, descriptor->imageInfos[i]);
									   }
									   auto bufferIdx = imageCount;
									   DescriptorSetLogic::WriteBuffer(writes, descriptor->set, bufferIdx,
																	   bindings[bufferIdx].descriptorType, descriptor->bufferInfos[0]);
								   });
	}
}