
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/descriptor_pool_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Render {

	void DescriptorPoolLogic::Create(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		createInfo.maxSets = 10;

		std::vector<VkDescriptorPoolSize> sizes = {
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10},
			{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4},
		};
		createInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
		createInfo.pPoolSizes = sizes.data();

		VkDescriptorPool vkDescriptorPool;
		auto ret = vkCreateDescriptorPool(logicalDevice, &createInfo, nullptr, &vkDescriptorPool);
		CheckRet(ret, "vkCreateDescriptorPool");

		global->descriptorPool = vkDescriptorPool;
	}

	void DescriptorPoolLogic::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& descriptorPool = global->descriptorPool;
		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	}

}