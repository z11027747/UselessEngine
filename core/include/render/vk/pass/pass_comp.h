#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render {

	struct Attachment final {
	};

	struct Pass final {
		
		VkRenderPass renderPass;

		std::vector<VkAttachmentDescription> descrpitions;
		std::vector<VkAttachmentReference> references;
	};


}