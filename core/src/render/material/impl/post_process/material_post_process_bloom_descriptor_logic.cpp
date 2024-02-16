
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
	void MaterialPostProcessBloomDescriptorLogic::CreateSetLayout(Context *context,
																  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		VkDescriptorSetLayoutBinding gaussBlurAttachment = {
			0, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(gaussBlurAttachment);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
		graphicsPipeline->subpass = 3;
	}

	void MaterialPostProcessBloomDescriptorLogic::AllocateAndUpdate(Context *context,
																	std::shared_ptr<MaterialInstance> materialInstance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
		auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);
		descriptor->set = descriptorSet;

		VkDescriptorImageInfo inputAttachmentInfo = {
			global->globalSamplerClamp,
			postProcessPass->inputImage2ds[2]->vkImageView,
			postProcessPass->inputImage2ds[2]->layout};
		descriptor->imageInfos.push_back(inputAttachmentInfo);

		DescriptorSetLogic::Update(context,
								   [=](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   auto &bindings = postProcessPipeline->descriptorBindings;
									   DescriptorSetLogic::WriteImage(writes, descriptor->set, 0,
																	  bindings[0].descriptorType, descriptor->imageInfos[0]);
								   });

		materialInstance->descriptor = descriptor;
	}
	void MaterialPostProcessBloomDescriptorLogic::Destroy(Context *context,
														  std::shared_ptr<MaterialInstance> materialInstance)
	{
	}
}