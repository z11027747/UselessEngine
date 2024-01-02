#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include "render/vk/image/image_comp.h"

namespace Render
{
	struct Pass final
	{
		std::vector<VkAttachmentDescription> attachmentDescriptions;

		VkAttachmentReference colorAttachmentReference;
		VkAttachmentReference depthAttachmentReference;

		std::vector<std::shared_ptr<Image>> colorImage2ds;
		std::vector<std::shared_ptr<Image>> depthImage2ds;

		VkClearColorValue clearColorValue;
		VkClearDepthStencilValue clearDepthValue;

		VkSubpassDescription subpassDescription;
		VkSubpassDependency subpassDependency;
		VkRenderPass renderPass;

		std::vector<VkFramebuffer> frameBuffers;
	};

}