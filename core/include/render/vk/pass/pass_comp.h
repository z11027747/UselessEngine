#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <string>
#include "render/vk/image/image_comp.h"

namespace Render
{
	struct Pass final
	{
		std::string name;

		std::vector<VkAttachmentDescription> attachmentDescriptions;

		VkAttachmentReference colorAttachmentReference;
		VkAttachmentReference depthAttachmentReference;
		VkAttachmentReference resolveAttachmentReference;

		bool isGetSwapchainImage;
		std::vector<std::shared_ptr<Image>> colorImage2ds;
		std::shared_ptr<Image> depthImage2d;
		std::shared_ptr<Image> resolveImage2d;
		VkSampleCountFlagBits msaaSamples{VK_SAMPLE_COUNT_1_BIT};

		VkClearColorValue clearColorValue;
		VkClearDepthStencilValue clearDepthValue;

		std::vector<VkSubpassDependency> subpassDependencies;
		VkSubpassDescription subpassDescription;
		VkRenderPass renderPass;

		std::vector<VkFramebuffer> frameBuffers;
	};
}