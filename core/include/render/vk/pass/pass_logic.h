#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "render/vk/pass/pass_comp.h"
#include "render/vk/image/image_logic.h"

class Context;

namespace Render
{
	class PassLogic final
	{
	public:
		static void Create(Context *, std::shared_ptr<Pass>);
		static void Destroy(Context *, std::shared_ptr<Pass>);

		// attachment
		static void CreateColorAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
										  VkSampleCountFlagBits,
										  VkImageLayout, VkImageLayout,
										  uint32_t = 0, VkImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		static void CreateDepthAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
										  VkSampleCountFlagBits, uint32_t = 1);
		static void CreateResolveAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
											VkImageLayout, VkImageLayout, uint32_t = 2);
		static void CreateInputAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
										  uint32_t, VkImageLayout);

		// subpass
		static void SetSubpassCount(Context *, std::shared_ptr<Pass>, uint32_t);
		static void SetSubpassDescription(Context *, std::shared_ptr<Pass>, uint32_t);

		static void AddSubpassDependency(Context *, std::shared_ptr<Pass>,
										 uint32_t, uint32_t,
										 VkPipelineStageFlags, VkAccessFlags,
										 VkPipelineStageFlags, VkAccessFlags);
	};

	class FramebufferLogic final
	{
	public:
		static void Create(Context *, std::shared_ptr<Pass>);
		static void Destroy(Context *, std::shared_ptr<Pass>);

		// image2d
		static void GetSwapchainImage2ds(Context *, std::shared_ptr<Pass>);
		static void CreateColorImage2d(Context *, std::shared_ptr<Pass>, VkSampleCountFlagBits);
		static void CreateDepthImage2d(Context *, std::shared_ptr<Pass>, VkSampleCountFlagBits);
		static void CreateResolveImage2d(Context *, std::shared_ptr<Pass>, VkSampleCountFlagBits);
		static void CreatePostProcessImage2d(Context *, std::shared_ptr<Pass>, uint32_t = 4);

		static void DestroyColorImage2ds(Context *, std::shared_ptr<Pass>);
		static void DestroyDepthImage2d(Context *, std::shared_ptr<Pass>);
		static void DestroyResolveImage2d(Context *, std::shared_ptr<Pass>);

		static void BeginRenderPass(Context *, uint32_t, std::shared_ptr<Pass>);
		static void EndRenderPass(Context *, uint32_t);
	};
}