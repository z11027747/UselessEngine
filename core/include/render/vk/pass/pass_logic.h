#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "render/vk/pass/pass_comp.h"

class Context;

namespace Render {

	class PassLogic final {
	public:

		static Pass Create(Context*, std::vector<Attachment> attachments);

		static void Destroy(Context*);

		static Attachment CreateColorAttachment(Context*);
		static Attachment CreateDepthAttachment(Context*);

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