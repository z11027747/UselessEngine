
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_skybox_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialSkyboxDescriptorLogic::CreateSetLayout(Context *context,
														std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		VkDescriptorSetLayoutBinding cubeMap = {
			0, // binding
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT};

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(cubeMap);

		graphicsPipeline->descriptorSetLayout = DescriptorSetLayoutLogic::Create(context, bindings);
	}
	void MaterialSkyboxDescriptorLogic::AllocateAndUpdate(Context *context,
														  std::shared_ptr<MaterialInstance> instance)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &info = instance->info;
		auto &graphicsPipeline = global->pipelineMap[info->pipelineName];
		auto &descriptorSetLayout = graphicsPipeline->descriptorSetLayout;

		auto descriptor = std::make_shared<Descriptor>();

		auto descriptorSet = DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
		descriptor->set = descriptorSet;

		VkDescriptorImageInfo imageInfo = {
			global->globalSamplerClamp,
			instance->images[0]->vkImageView,
			instance->images[0]->layout};

		descriptor->imageInfos.push_back(imageInfo);

		instance->descriptor = descriptor;

		DescriptorSetLogic::Update(context,
								   [&descriptor](std::vector<VkWriteDescriptorSet> &writes)
								   {
									   DescriptorSetLogic::WriteImage(writes,
																	  descriptor->set, 0, descriptor->imageInfos[0]);
								   });
	}
	void MaterialSkyboxDescriptorLogic::Destroy(Context *context,
												std::shared_ptr<MaterialInstance> instance)
	{
	}
}