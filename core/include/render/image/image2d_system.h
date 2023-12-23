#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/image/image_comp.h"

class Context;

namespace Render {

	class Image2DSystem final {
	public:

		static std::shared_ptr<Image2D> Create(Context*,
			VkFormat format, VkExtent2D extent,
			VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags propertiesFlags
		);

		static void Destroy(Context*,
			std::shared_ptr<Image2D> image2d
		);

		static void CreateView(Context* context,
			std::shared_ptr<Image2D> image2d,
			VkImageAspectFlags aspectMask
		);

		static void DestroyView(Context* context,
			std::shared_ptr<Image2D> image2d
		);

		static void TransitionLayout(Context*,
			std::shared_ptr<Image2D> image2d,
			VkImageLayout oldLayout, VkImageLayout newLayout,
			VkImageAspectFlags aspectMask,
			VkAccessFlags srcAccessMas, VkAccessFlags dstAccessMas,
			VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage
		);

	};


}