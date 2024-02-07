
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	VkDescriptorSet DescriptorSetLogic::AllocateOne(Context *context, VkDescriptorSetLayout descriptorSetLayout)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &descriptorPool = global->descriptorPool;

		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = descriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &descriptorSetLayout;

		VkDescriptorSet descriptorSet;
		auto ret = vkAllocateDescriptorSets(logicalDevice, &allocateInfo, &descriptorSet);
		CheckRet(ret, "vkAllocateDescriptorSets");

		return descriptorSet;
	}

	std::vector<VkDescriptorSet> DescriptorSetLogic::Allocate(Context *context,
															  VkDescriptorSetLayout descriptorSetLayout, uint32_t count)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &descriptorPool = global->descriptorPool;

		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = descriptorPool;
		allocateInfo.descriptorSetCount = count;

		std::vector<VkDescriptorSetLayout> layouts(count, descriptorSetLayout);
		allocateInfo.pSetLayouts = layouts.data();

		std::vector<VkDescriptorSet> descriptorSets(count);
		auto ret = vkAllocateDescriptorSets(logicalDevice, &allocateInfo, descriptorSets.data());
		CheckRet(ret, "vkAllocateDescriptorSets");

		return descriptorSets;
	}
}