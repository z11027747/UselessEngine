#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/image/image_comp.h"

class Context;

namespace Render {

	class ImageSystem final {
	public:

		//创建图像
		static std::shared_ptr<Image2D> Create(Context*,
			VkFormat format, VkExtent2D extent,
			VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags propertiesFlags
		);

		//创建图像视图
		static void CreateImageView(Context* context,
			std::shared_ptr<Image2D> image2d,
			VkImageAspectFlags aspectMask
		);

		//销毁图像
		static void Destroy(Context*,
			std::shared_ptr<Image2D> image2d
		);
	};


}