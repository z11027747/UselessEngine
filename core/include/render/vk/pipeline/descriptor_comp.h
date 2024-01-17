#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>

namespace Render
{
	struct Descriptor final
	{
		VkDescriptorSet set;
		VkDescriptorBufferInfo bufferInfo;
		std::vector<VkDescriptorImageInfo> imageInfos;
	};
}