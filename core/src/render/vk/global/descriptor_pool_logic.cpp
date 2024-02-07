
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "context.hpp"
#include "engine_object.hpp"

namespace Render
{
	void DescriptorPoolLogic::Create(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;

		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		createInfo.maxSets = 1000;

		std::vector<VkDescriptorPoolSize> sizes = {
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100},
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100},
		};
		createInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
		createInfo.pPoolSizes = sizes.data();

		VkDescriptorPool vkDescriptorPool;
		auto ret = vkCreateDescriptorPool(logicalDevice, &createInfo, nullptr, &vkDescriptorPool);
		CheckRet(ret, "vkCreateDescriptorPool");

		global->descriptorPool = vkDescriptorPool;
	}

	void DescriptorPoolLogic::Destroy(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &descriptorPool = global->descriptorPool;

		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	}

}