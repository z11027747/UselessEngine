
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/descriptor_pool_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "context.h"
#include "base.h"

namespace Render {

	void DescriptorPoolLogic::Create(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.maxSets = maxFrameInFlight + 1;

		std::vector<VkDescriptorPoolSize> sizes(2);

		sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		sizes[0].descriptorCount = 10;

		sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		sizes[1].descriptorCount = 4;

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