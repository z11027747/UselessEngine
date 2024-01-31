
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/impl/material_post_process_logic.h"
#include "context.h"

namespace Render
{
	void MaterialPostProcessBloomDescriptorLogic::CreateSetLayout(Context *context,
																  std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		VkDescriptorSetLayoutBinding colorAttachment = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(colorAttachment);

		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}
}