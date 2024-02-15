
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/impl/material_post_process_logic.h"
#include "context.hpp"

namespace Render
{
	void MaterialPostProcessDescriptorLogic::CreateSetLayout(Context *context,
															 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		VkDescriptorSetLayoutBinding resolveImage = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding toonMappingAttachment = {
			1, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding gaussBlurAttachment = {
			2, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};
		VkDescriptorSetLayoutBinding bloomAttachment = {
			3, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(resolveImage);
		bindings.push_back(toonMappingAttachment);
		bindings.push_back(gaussBlurAttachment);
		bindings.push_back(bloomAttachment);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);

		// subpass index
		if (graphicsPipeline->name == Define::Pipeline::PostProcess_ToonMapping)
		{
			graphicsPipeline->subpass = 0;
		}
		else if (graphicsPipeline->name == Define::Pipeline::PostProcess_GaussBlur)
		{
			graphicsPipeline->subpass = 1;
		}
		else if (graphicsPipeline->name == Define::Pipeline::PostProcess_Bloom)
		{
			graphicsPipeline->subpass = 2;
		}
		else if (graphicsPipeline->name == Define::Pipeline::PostProcess_Global)
		{
			graphicsPipeline->subpass = 3;
		}
	}

	constexpr int imageCount = 4; // blit + toonmapping+gaussblur+bloom

	void MaterialPostProcessDescriptorLogic::AllocateAndUpdate(Context *context,
															   std::shared_ptr<MaterialInstance> materialInstance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
		auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Global];

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);
		descriptor->set = descriptorSet;

		VkDescriptorImageInfo blitImageInfo = {
			global->globalSamplerClamp,
			postProcessPass->colorImage2ds[1]->vkImageView,
			postProcessPass->colorImage2ds[1]->layout};
		descriptor->imageInfos.push_back(blitImageInfo);

		for (auto i = 0; i < imageCount - 1; i++)
		{
			VkDescriptorImageInfo inputAttachmentInfo = {
				global->globalSamplerClamp,
				postProcessPass->inputImage2ds[i]->vkImageView,
				postProcessPass->inputImage2ds[i]->layout};
			descriptor->imageInfos.push_back(inputAttachmentInfo);
		}

		DescriptorSetLogic::Update(context,
								   [=](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   auto &bindings = postProcessPipeline->descriptorBindings;
									   for (auto i = 0; i < imageCount; i++)
									   {
										   DescriptorSetLogic::WriteImage(writes, descriptor->set, i,
																		  bindings[i].descriptorType, descriptor->imageInfos[i]);
									   }
								   });

		materialInstance->descriptor = descriptor;
	}
	void MaterialPostProcessDescriptorLogic::Destroy(Context *context,
													 std::shared_ptr<MaterialInstance> materialInstance)
	{
	}
}