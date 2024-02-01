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
		static void CreateColorAttachment(Context *,
										  std::shared_ptr<Pass>,
										  VkSampleCountFlagBits,
										  VkImageLayout, VkImageLayout, VkImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		static void CreateDepthAttachment(Context *, std::shared_ptr<Pass>,
										  VkSampleCountFlagBits, uint32_t = 1);
		static void CreateResolveAttachment(Context *, std::shared_ptr<Pass>,
											VkImageLayout, VkImageLayout);

		static void GetSwapchainImage2ds(Context *, std::shared_ptr<Pass>);
		static void CreateColorImage2d(Context *, std::shared_ptr<Pass>, VkSampleCountFlagBits);
		static void CreateDepthImage2d(Context *, std::shared_ptr<Pass>, VkSampleCountFlagBits);
		static void CreateResolveImage2d(Context *, std::shared_ptr<Pass>, VkSampleCountFlagBits);

		static void DestroyColorImage2ds(Context *, std::shared_ptr<Pass>);
		static void DestroyDepthImage2d(Context *, std::shared_ptr<Pass>);
		static void DestroyResolveImage2d(Context *, std::shared_ptr<Pass>);

		static void CreatePostProcessImage2d(Context *, std::shared_ptr<Pass>, uint32_t = 4);

		static void AddSubpassDependency(Context *,
										 std::shared_ptr<Pass>,
										 uint32_t, uint32_t,
										 VkPipelineStageFlags, VkAccessFlags,
										 VkPipelineStageFlags, VkAccessFlags);

		static void SetSubPassDescription(Context *,
										  std::shared_ptr<Pass>);

		static void Create(Context *, std::shared_ptr<Pass>);
		static void Destroy(Context *, std::shared_ptr<Pass>);
	};

	class FramebufferLogic final
	{
	public:
		static void Create(Context *, std::shared_ptr<Pass>);
		static void Destroy(Context *, std::shared_ptr<Pass>);

		static void BeginRenderPass(Context *, uint32_t, std::shared_ptr<Pass>);
		static void EndRenderPass(Context *, uint32_t);
	};
}