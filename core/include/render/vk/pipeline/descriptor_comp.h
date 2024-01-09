#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace Render
{
	struct Descriptor final
	{
		VkDescriptorSet set;
		VkDescriptorBufferInfo bufferInfo;
		VkDescriptorImageInfo image0Info;
		VkDescriptorImageInfo image1Info;
	};
}