#pragma once

#include <vulkan/vulkan.h>

namespace Render {

	struct ImageInfo final {
		VkFormat format;
		VkExtent3D extent;
		VkImageAspectFlags aspectMask;

		//image
		VkImageTiling tiling;
		VkImageUsageFlags usage;
		VkImageCreateFlags flags;
		uint32_t arrayLayers;
		VkImageViewType viewType;

		//memory
		VkMemoryPropertyFlags propertiesFlags;

		//layout
		VkImageLayout oldLayout, newLayout;
		VkAccessFlags srcAccessMask, dstAccessMask;
		VkPipelineStageFlags srcStage, dstStage;
	};

	struct Image final {
		VkFormat fomat;
		VkExtent3D extent;
		VkImageAspectFlags aspectMask;
		uint32_t layerCount;
		VkImageLayout layout;

		VkImage vkImage;
		VkDeviceSize size;
		VkDeviceMemory vkDeviceMemory;

		VkImageView vkImageView;
	};

}