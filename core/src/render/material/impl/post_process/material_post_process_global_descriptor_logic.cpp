
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
	void MaterialPostProcessGlobalDescriptorLogic::CreateSetLayout(Context *context,
																   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		VkDescriptorSetLayoutBinding SSAOAttachment = {
			0, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
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
		bindings.push_back(SSAOAttachment);
		bindings.push_back(toonMappingAttachment);
		bindings.push_back(gaussBlurAttachment);
		bindings.push_back(bloomAttachment);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
		graphicsPipeline->subpass = 4;
	}

	constexpr int imageCount = 4; // SSAO+toonmapping+gaussblur+bloom

	void MaterialPostProcessGlobalDescriptorLogic::AllocateAndUpdate(Context *context,
																	 std::shared_ptr<MaterialInstance> materialInstance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
		auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Global];

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);
		descriptor->set = descriptorSet;

		for (auto i = 0; i < imageCount; i++)
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
	void MaterialPostProcessGlobalDescriptorLogic::Destroy(Context *context,
														   std::shared_ptr<MaterialInstance> materialInstance)
	{
	}
}