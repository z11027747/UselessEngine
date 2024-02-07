
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
		VkDescriptorSetLayoutBinding bloomAttachment = {
			2, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(resolveImage);
		bindings.push_back(toonMappingAttachment);
		bindings.push_back(bloomAttachment);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);

		// subpass index
		if (graphicsPipeline->name == Define::Pipeline::PostProcess_ToonMapping)
		{
			graphicsPipeline->subpass = 0;
		}
		else if (graphicsPipeline->name == Define::Pipeline::PostProcess_Bloom)
		{
			graphicsPipeline->subpass = 1;
		}
		else if (graphicsPipeline->name == Define::Pipeline::PostProcess_Global)
		{
			graphicsPipeline->subpass = 2;
		}
	}
}