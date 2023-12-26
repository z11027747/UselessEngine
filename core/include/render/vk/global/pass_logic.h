#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render {

	class PassLogic final {
	public:

		static void Create(Context*);
		static void Destroy(Context*);

		static void MakeColorAttachment(Context*,
			VkAttachmentDescription& colorAttachmentDescription,
			VkAttachmentReference& colorAttachmentReference
		);
		static void MakeDepthAttachment(Context*,
			VkAttachmentDescription& depthAttachmentDescription,
			VkAttachmentReference& depthAttachmentReference
		);
	};

}