#pragma once

#include <vulkan/vulkan.h>

namespace Render
{
	struct ImageCreateInfo final
	{
		// info
		VkFormat format;
		VkExtent3D extent;
		VkImageAspectFlags aspectMask;
		VkImageTiling tiling;
		VkImageUsageFlags usage;
		VkImageCreateFlags flags;
		uint32_t layerCount;
		VkImageViewType viewType;
		uint32_t mipLevels;
		VkSampleCountFlagBits samplers;

		// memory
		VkMemoryPropertyFlags propertiesFlags;

		// layout
		VkImageLayout oldLayout, newLayout;
	};

	struct Image final
	{
		VkFormat format;
		VkExtent3D extent;
		VkImageAspectFlags aspectMask;
		uint32_t layerCount{1};
		VkImageLayout layout{VK_IMAGE_LAYOUT_UNDEFINED};
		uint32_t mipLevels{1};

		VkImage vkImage;
		VkDeviceSize size;
		VkDeviceMemory vkDeviceMemory;

		VkImageView vkImageView;
	};

}