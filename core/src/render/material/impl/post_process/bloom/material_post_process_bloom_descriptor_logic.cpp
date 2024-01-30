
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

		auto descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);

		auto &resolveImage2d = global->passMap[Define::Pass::Main]->resolveImage2d;
		VkDescriptorImageInfo imageInfo = {
			global->globalSamplerClamp,
			resolveImage2d->vkImageView,
			resolveImage2d->layout};

		auto descriptor = std::make_shared<Descriptor>();
		descriptor->set = descriptorSet;
		descriptor->imageInfos.push_back(imageInfo);

		graphicsPipeline->descriptorSetLayout = descriptorSetLayout;
		graphicsPipeline->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
								   [&descriptor](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   DescriptorSetLogic::WriteImage(writes,
																	  descriptor->set, 0, descriptor->imageInfos[0]);
								   });
	}
}