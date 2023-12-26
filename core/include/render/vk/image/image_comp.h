#pragma once

#include <vulkan/vulkan.h>

namespace Render {

	struct Image2D final {

		VkFormat fomat;
		VkExtent2D extent;

		VkImage vkImage;
		VkDeviceMemory vkDeviceMemory;
		VkImageView vkImageView;

	};

}