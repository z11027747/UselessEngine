
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/pipeline_layout_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "context.h"

namespace Render {

	std::vector<VkDescriptorSet> DescriptorSetLogic::Allocate(Context* context,
		VkDescriptorSetLayout descriptorSetLayout)
	{
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;
		auto& descriptorPool = global->descriptorPool;

		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = descriptorPool;
		allocateInfo.descriptorSetCount = maxFrameInFlight;

		std::vector<VkDescriptorSetLayout> layouts(maxFrameInFlight, descriptorSetLayout);
		allocateInfo.pSetLayouts = layouts.data();

		std::vector<VkDescriptorSet> descriptorSets(maxFrameInFlight);
		auto ret = vkAllocateDescriptorSets(logicalDevice, &allocateInfo, descriptorSets.data());
		CheckRet(ret, "vkAllocateDescriptorSets");

		return descriptorSets;

	}

}