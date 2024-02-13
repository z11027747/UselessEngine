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
		
		VkSampleCountFlagBits msaaSamples{VK_SAMPLE_COUNT_1_BIT};
	};

	struct Pass final
	{
		std::string name;
		
		std::vector<VkAttachmentDescription> attachmentDescriptions;
		std::vector<VkClearValue> clearValues;

		std::vector<std::shared_ptr<Subpass>> subpasses;
		std::vector<VkSubpassDescription> subpassDescriptions;
		std::vector<VkSubpassDependency> subpassDependencies;

		bool isGetSwapchainImage;
		std::vector<std::shared_ptr<Image>> colorImage2ds;
		std::vector<std::shared_ptr<Image>> depthImage2ds;
		std::shared_ptr<Image> resolveImage2d;
		std::vector<std::shared_ptr<Image>> inputImage2ds;

		VkOffset2D offset{0, 0};
		VkExtent2D extent{0, 0};

		VkRenderPass renderPass;

		std::vector<VkFramebuffer> frameBuffers;
	};
}