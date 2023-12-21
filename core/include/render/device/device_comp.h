#pragma once

#include <vulkan/vulkan.h>

namespace Render {

	struct Device final {

		VkPhysicalDevice physicalDevice;
		uint32_t physicalQueueFamilyIndex;
		VkDevice logicalDevice;
		VkQueue logicalQueue;
	};

}
