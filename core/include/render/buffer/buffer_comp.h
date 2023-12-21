#pragma once

#include <vulkan/vulkan.h>

namespace Render {

	struct Buffer final {

		VkBuffer vkBuffer;
		VkDeviceMemory vkDeviceMemory;

	};

}