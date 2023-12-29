#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <functional>
#include "render/vk/pipeline/pipeline_comp.h"
#include "context.h"

//class Context;

namespace Render {

	class DescriptorSetLogic final {
	public:

		static VkDescriptorSet AllocateOne(Context*,
			VkDescriptorSetLayout);

		static std::vector<VkDescriptorSet> Allocate(Context*,
			VkDescriptorSetLayout);

		static void Update(Context* context,
			std::function<void(std::vector<VkWriteDescriptorSet>&)> func
		) {
			auto& renderGlobalEO = context->renderGlobalEO;

			auto global = renderGlobalEO->GetComponent<Global>();
			auto& logicalDevice = global->logicalDevice;

			std::vector<VkWriteDescriptorSet> writes;

			func(writes);

			vkUpdateDescriptorSets(logicalDevice,
				static_cast<uint32_t>(writes.size()), writes.data(),
				0, nullptr);
		}

		static void WriteBuffer(Context*,
			std::vector<VkWriteDescriptorSet>& writes,
			std::shared_ptr<Descriptor> descriptor
		) {
			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = descriptor->set;
			write.dstBinding = 0;
			write.dstArrayElement = 0;
			write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write.descriptorCount = 1;
			write.pBufferInfo = &descriptor->bufferInfo;

			writes.push_back(write);
		}

		static void WriteImage(Context*,
			std::vector<VkWriteDescriptorSet>& writes,
			std::shared_ptr<Descriptor> descriptor
		) {
			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = descriptor->set;
			write.dstBinding = 0;
			write.dstArrayElement = 0;
			write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.descriptorCount = 1;
			write.pImageInfo = &descriptor->imageInfo;

			writes.push_back(write);
		}
	};


}