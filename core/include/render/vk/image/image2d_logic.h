#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/vk/image/image_comp.h"
#include "render/vk/buffer/buffer_comp.h"

class Context;

namespace Render {

	class Image2DLogic final {
	public:

		static std::shared_ptr<Image2D> CreateByInfo(Context*,
			Image2DInfo&);

		static void Create(Context*,
			std::shared_ptr<Image2D>,
			VkFormat, VkExtent2D,
			VkImageTiling, VkImageUsageFlags,
			VkMemoryPropertyFlags);
		static void Destroy(Context*,
			std::shared_ptr<Image2D>);

		static void CreateView(Context*,
			std::shared_ptr<Image2D>,
			VkImageAspectFlags);
		static void DestroyView(Context*,
			std::shared_ptr<Image2D>);

		static void TransitionLayout(Context*,
			std::shared_ptr<Image2D>,
			VkImageLayout, VkImageLayout,
			VkImageAspectFlags,
			VkAccessFlags, VkAccessFlags,
			VkPipelineStageFlags, VkPipelineStageFlags);

		static void CopyBuffer(Context*,
			std::shared_ptr<Image2D>,
			std::shared_ptr<Buffer>);

	};


}