#pragma once

#include <vulkan/vulkan.h>

namespace Render {

	struct Image2D final {

		VkFormat fomat;
		VkExtent2D extent;
		VkImageAspectFlags aspectMask;

		VkImage vkImage;
		VkDeviceMemory vkDeviceMemory;
		VkImageView vkImageView;
	};

	struct Image2DInfo final {

		VkFormat format;
		VkExtent2D extent;

		VkImageTiling tiling;
		VkImageUsageFlags usage;
		VkMemoryPropertyFlags properitesFlags;

		VkImageAspectFlags aspectMask;

		VkImageLayout oldLayout, newLayout;
		VkAccessFlags srcAccessMask, dstAccessMask;
		VkPipelineStageFlags srcStage, dstStage;

	};

}