
#include <vulkan/vulkan.h>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/material/impl/material_water_logic.h"
#include "context.hpp"

namespace Render
{
	void MaterialWaterDescriptorLogic::CreateSetLayout(Context *context,
													   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding depth = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding materialUBO = {
			1, // binding
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_VERTEX_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(depth);
		bindings.push_back(materialUBO);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}
	void MaterialWaterDescriptorLogic::AllocateAndUpdate(Context *context,
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

		//TODO
		auto &shadowPass = global->passMap[Define::Pass::Shadow];

		VkDescriptorImageInfo imageInfo = {
			global->globalSamplerClampLinear,
			shadowPass->depthImage2ds[0]->vkImageView,
			shadowPass->depthImage2ds[0]->layout};
		descriptor->imageInfos.push_back(imageInfo);

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
									   DescriptorSetLogic::WriteImage(writes, descriptor->set, 0,
																	  bindings[0].descriptorType, descriptor->imageInfos[0]);
									   DescriptorSetLogic::WriteBuffer(writes, descriptor->set, 1,
																	   bindings[1].descriptorType, descriptor->bufferInfos[0]);
								   });
	}
	void MaterialWaterDescriptorLogic::Destroy(Context *context,
											   std::shared_ptr<MaterialData> data)
	{
	}
}