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
												   ImageCreateInfo &);

		static void Create(Context *, std::shared_ptr<Image>, ImageCreateInfo &);
		static void Destroy(Context *, std::shared_ptr<Image>);

		static void CreateView(Context *, std::shared_ptr<Image>,
							   VkImageViewType, VkImageAspectFlags, uint32_t, uint32_t);
		static void DestroyView(Context *, std::shared_ptr<Image>);

		static void TransitionLayout(Context *, std::shared_ptr<Image>,
									 VkImageLayout, VkImageLayout, uint32_t,
									 bool singleTime = false);

		static void GenerateMipmapsAndTransitionLayout(Context *, std::shared_ptr<Image>,
													   VkImageLayout, VkImageLayout);

		static void CopyFromBuffer(Context *, std::shared_ptr<Image>,
								   std::shared_ptr<Buffer>);
		static void CopyFromImage(Context *, std::shared_ptr<Image>,
								  std::shared_ptr<Image>, bool singleTime = false);
	};
}