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
										  VkFormat, VkSampleCountFlagBits,
										  VkImageLayout, VkImageLayout,
										  VkClearColorValue && = {0.0f, 0.0f, 0.0f, 0.0f});
		static void SetColorAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
									   uint32_t = 0u, VkImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		static void SetInputAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
									   uint32_t, VkImageLayout);

		static void CreateDepthAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
										  VkSampleCountFlagBits,
										  VkImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
		static void CreateDepthStencilAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
												 VkSampleCountFlagBits,
												 VkImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
		static void SetDepthAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
									   uint32_t);

		static void CreateResolveAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
											VkImageLayout, VkImageLayout,
											VkClearColorValue && = {0.0f, 0.0f, 0.0f, 0.0f});
		static void SetResolveAttachment(Context *, std::shared_ptr<Pass>, uint32_t,
										 uint32_t = 2u);

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
		static void CreateDepthStencilImage2d(Context *, std::shared_ptr<Pass>, VkSampleCountFlagBits);
		static void CreateResolveImage2d(Context *, std::shared_ptr<Pass>, VkSampleCountFlagBits);
		static void CreateInputImage2d(Context *, std::shared_ptr<Pass>, VkFormat, VkSampleCountFlagBits = VK_SAMPLE_COUNT_1_BIT);
		static void CreateBlitImage2d(Context *, std::shared_ptr<Pass>, uint32_t = 4);

		static void DestroyColorImage2ds(Context *, std::shared_ptr<Pass>);
		static void DestroyDepthImage2d(Context *, std::shared_ptr<Pass>);
		static void DestroyResolveImage2d(Context *, std::shared_ptr<Pass>);
		static void DestroyInputImage2ds(Context *, std::shared_ptr<Pass>);

		static void BeginRenderPass(Context *, std::shared_ptr<Pass>);
		static void NextSubpass(Context *);
		static void EndRenderPass(Context *);
	};
}