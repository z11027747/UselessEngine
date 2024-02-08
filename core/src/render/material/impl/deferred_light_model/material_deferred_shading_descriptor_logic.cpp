
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_deferred_logic.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	void MaterialDeferredShadingDescriptorLogic::CreateSetLayout(Context *context,
																 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding shadowMap = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding positionAttachment = {
			1, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding normalAttachment = {
			2, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding colorAttachment = {
			3, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding materialAttachment = {
			4, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding pointlightAttachment = {
			5, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(shadowMap);
		bindings.push_back(positionAttachment);
		bindings.push_back(normalAttachment);
		bindings.push_back(colorAttachment);
		bindings.push_back(materialAttachment);
		bindings.push_back(pointlightAttachment);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);

		graphicsPipeline->subpass = 2;
	}

	constexpr int imageCount = 6; // shadow + gBuffer(position+normal+color+material+pointlight)

	void MaterialDeferredShadingDescriptorLogic::AllocateAndUpdate(Context *context,
																   std::shared_ptr<MaterialInstance> materialInstance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &deferredPass = global->passMap[Define::Pass::Deferred];
		auto &deferredLightingPipeline = global->pipelineMap[Define::Pipeline::Deferred_Shading];

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, deferredLightingPipeline->descriptorSetLayout);
		descriptor->set = descriptorSet;

		auto &shadowPass = global->passMap[Define::Pass::Shadow];
		auto depthImageSampler = SamplerLogic::CreateDepth(context);

		VkDescriptorImageInfo shadowImageInfo = {
			depthImageSampler,
			shadowPass->depthImage2d->vkImageView,
			shadowPass->depthImage2d->layout};
		descriptor->imageInfos.push_back(shadowImageInfo);

		for (auto i = 0; i < imageCount - 1; i++)
		{
			VkDescriptorImageInfo inputAttachmentInfo = {
				global->globalSamplerClamp,
				deferredPass->inputImage2ds[i]->vkImageView,
				deferredPass->inputImage2ds[i]->layout};
			descriptor->imageInfos.push_back(inputAttachmentInfo);
		}

		DescriptorSetLogic::Update(context,
								   [=](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   auto &bindings = deferredLightingPipeline->descriptorBindings;
									   for (auto i = 0; i < imageCount; i++)
									   {
										   DescriptorSetLogic::WriteImage(writes, descriptor->set, i,
																		  bindings[i].descriptorType, descriptor->imageInfos[i]);
									   }
								   });

		materialInstance->descriptor = descriptor;
	}
	void MaterialDeferredShadingDescriptorLogic::Destroy(Context *context,
														 std::shared_ptr<MaterialInstance> materialInstance)
	{
		auto &descriptor = materialInstance->descriptor;
		SamplerLogic::Destroy(context, descriptor->imageInfos[0].sampler);
	}
}