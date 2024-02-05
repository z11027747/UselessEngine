
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_deferred_light_model_logic.h"
#include "common/define.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialDeferredLightModelLightingDescriptorLogic::CreateSetLayout(Context *context,
																			std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding shadowMap = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding position = {
			1, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding normal = {
			2, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding color = {
			3, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		VkDescriptorSetLayoutBinding material = {
			4, // binding
			VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(shadowMap);
		bindings.push_back(position);
		bindings.push_back(normal);
		bindings.push_back(color);
		bindings.push_back(material);

		graphicsPipeline->descriptorBindings = bindings;
		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);

		graphicsPipeline->subpass = 1;
	}

	void MaterialDeferredLightModelLightingDescriptorLogic::AllocateAndUpdate(Context *context,
																			  std::shared_ptr<MaterialInstance> instance)
	{
	}
	void MaterialDeferredLightModelLightingDescriptorLogic::Destroy(Context *context,
																	std::shared_ptr<MaterialInstance> instance)
	{
	}
}