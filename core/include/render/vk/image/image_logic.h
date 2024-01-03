#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/vk/image/image_comp.h"
#include "render/vk/buffer/buffer_comp.h"

class Context;

namespace Render
{
	class ImageLogic final
	{
	public:
		static std::shared_ptr<Image> CreateByInfo(Context *,
												   ImageInfo &);

		static void Create(Context *,
						   std::shared_ptr<Image>,
						   ImageInfo &);
		static void Destroy(Context *, std::shared_ptr<Image>);

		static void CreateView(Context *,
							   std::shared_ptr<Image>,
							   VkImageViewType, VkImageAspectFlags, uint32_t);
		static void DestroyView(Context *, std::shared_ptr<Image>);

		static void TransitionLayout(Context *,
									 std::shared_ptr<Image>,
									 VkImageLayout, VkImageLayout,
									 VkAccessFlags, VkAccessFlags,
									 VkPipelineStageFlags, VkPipelineStageFlags);

		static void CopyBuffer(Context *,
							   std::shared_ptr<Image>,
							   std::shared_ptr<Buffer>);
		static void CopyImage(Context *,
							  std::shared_ptr<Image>,
							  std::shared_ptr<Image>);
	};

}