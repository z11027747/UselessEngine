#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <string>
#include "render/vk/image/image_comp.h"

namespace Render
{
	struct Subpass final
	{
		std::vector<VkAttachmentReference> colorAttachmentReferences;
		VkAttachmentReference depthAttachmentReference;
		VkAttachmentReference resolveAttachmentReference;
		std::vector<VkAttachmentReference> inputAttachmentReferences;
	};

	struct Pass final
	{
		std::string name;

		std::vector<VkAttachmentDescription> attachmentDescriptions;

		std::vector<Subpass> subpasses;
		std::vector<VkSubpassDescription> subpassDescriptions;

		bool isGetSwapchainImage;
		std::vector<std::shared_ptr<Image>> colorImage2ds;
		std::shared_ptr<Image> depthImage2d;
		std::shared_ptr<Image> resolveImage2d;
		VkSampleCountFlagBits msaaSamples{VK_SAMPLE_COUNT_1_BIT};

		VkOffset2D offset{0, 0};
		VkExtent2D extent{0, 0};

		VkClearColorValue clearColorValue;
		VkClearDepthStencilValue clearDepthValue;

		std::vector<VkSubpassDependency> subpassDependencies;
		VkRenderPass renderPass;

		std::vector<VkFramebuffer> frameBuffers;
	};
}