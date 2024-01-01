#pragma once

#include <vulkan/vulkan.h>

namespace Render {

	struct Buffer final {
		VkDeviceSize size;

		VkBuffer vkBuffer;
		VkDeviceMemory vkDeviceMemory;
	};

}