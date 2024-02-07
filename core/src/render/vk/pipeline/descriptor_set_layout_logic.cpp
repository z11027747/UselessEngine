
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	VkDescriptorSetLayout DescriptorSetLayoutLogic::Create(Context *context,
														   std::vector<VkDescriptorSetLayoutBinding> &bindings)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		createInfo.pBindings = bindings.data();

		VkDescriptorSetLayout descriptorSetLayout;
		auto ret = vkCreateDescriptorSetLayout(logicalDevice, &createInfo, nullptr, &descriptorSetLayout);
		CheckRet(ret, "vkCreateDescriptorSetLayout");

		return descriptorSetLayout;
	}

	void DescriptorSetLayoutLogic::Destroy(Context *context,
										   VkDescriptorSetLayout descriptorSetLayout)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	}
}