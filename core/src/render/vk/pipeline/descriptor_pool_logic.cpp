
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/vk/pipeline/descriptor_pool_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "context.h"
#include "base.h"

namespace Render {

	void DescriptorPoolLogic::Create(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.maxSets = maxFrameInFlight;

		std::vector<VkDescriptorPoolSize> sizes(2);

		sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		sizes[0].descriptorCount = 2;

		sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		sizes[1].descriptorCount = 1;

		createInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
		createInfo.pPoolSizes = sizes.data();

		VkDescriptorPool vkDescriptorPool;
		auto ret = vkCreateDescriptorPool(logicalDevice, &createInfo, nullptr, &vkDescriptorPool);
		CheckRet(ret, "vkCreateDescriptorPool");

		graphicsPipeline->descriptorPool = vkDescriptorPool;
	}

	void DescriptorPoolLogic::Destroy(Context* context,
		std::shared_ptr<GraphicsPipeline> graphicsPipeline
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& descriptorPool = graphicsPipeline->descriptorPool;
		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	}

}