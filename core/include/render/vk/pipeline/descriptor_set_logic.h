#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <functional>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	class DescriptorSetLogic final
	{
	public:
		static VkDescriptorSet AllocateOne(Context *,
										   VkDescriptorSetLayout);

		static std::vector<VkDescriptorSet> Allocate(Context *,
													 VkDescriptorSetLayout, uint32_t);

		static void Update(Context *context,
						   std::function<void(std::vector<VkWriteDescriptorSet> &)> func)
		{
			auto &globalEO = context->renderGlobalEO;
			auto global = globalEO->GetComponent<Global>();
			auto &logicalDevice = global->logicalDevice;

			std::vector<VkWriteDescriptorSet> writes;

			func(writes);

			vkUpdateDescriptorSets(logicalDevice,
								   static_cast<uint32_t>(writes.size()), writes.data(),
								   0, nullptr);
		}

		static void WriteBuffer(std::vector<VkWriteDescriptorSet> &writes,
								VkDescriptorSet set, uint32_t binding, VkDescriptorBufferInfo &bufferInfo)
		{
			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = set;
			write.dstBinding = binding;
			write.dstArrayElement = 0;
			write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write.descriptorCount = 1;
			write.pBufferInfo = &bufferInfo;

			writes.push_back(write);
		}

		static void WriteImage(std::vector<VkWriteDescriptorSet> &writes,
							   VkDescriptorSet set, uint32_t binding, VkDescriptorImageInfo &imageInfo)
		{
			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = set;
			write.dstBinding = binding;
			write.dstArrayElement = 0;
			write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.descriptorCount = 1;
			write.pImageInfo = &imageInfo;

			writes.push_back(write);
		}
	};
}