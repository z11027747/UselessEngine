#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <functional>
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

		static void WriteBuffer(Context *,
								std::vector<VkWriteDescriptorSet> &writes,
								std::shared_ptr<Descriptor> descriptor, int binding = 0)
		{
			VkWriteDescriptorSet write = {};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = descriptor->set;
			write.dstBinding = binding;
			write.dstArrayElement = 0;
			write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write.descriptorCount = 1;
			write.pBufferInfo = &descriptor->bufferInfo;

			writes.push_back(write);
		}

		static void WriteImage(Context *,
							   std::vector<VkWriteDescriptorSet> &writes,
							   std::shared_ptr<Descriptor> descriptor)
		{
			VkWriteDescriptorSet write0 = {};
			write0.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write0.dstSet = descriptor->set;
			write0.dstBinding = 0;
			write0.dstArrayElement = 0;
			write0.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write0.descriptorCount = 1;
			write0.pImageInfo = &descriptor->image0Info;

			writes.push_back(write0);

			if (descriptor->image1Info.imageView != nullptr)
			{
				VkWriteDescriptorSet write1 = {};
				write1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write1.dstSet = descriptor->set;
				write1.dstBinding = 1;
				write1.dstArrayElement = 0;
				write1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				write1.descriptorCount = 1;
				write1.pImageInfo = &descriptor->image1Info;

				writes.push_back(write1);
			}
		}
	};
}