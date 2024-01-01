#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace Render {

	struct Pass final {

		VkAttachmentDescription colorAttachmentDescription;
		VkAttachmentReference colorAttachmentReference;
		VkAttachmentDescription depthAttachmentDescription;
		VkAttachmentReference depthAttachmentReference;

		std::vector<std::shared_ptr<Image>> colorImage2ds;
		std::vector<std::shared_ptr<Image>> depthImage2ds;

		std::vector<VkClearColorValue> clearColorValues;
		std::vector<VkClearDepthStencilValue> clearDepthValues;

		VkSubpassDescription subpassDescription;
		VkSubpassDependency subpassDependency;
		VkRenderPass renderPass;

		std::vector<VkFramebuffer> frameBuffers;
	};

}