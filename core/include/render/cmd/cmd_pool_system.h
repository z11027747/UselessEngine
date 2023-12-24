#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render {

	class CmdPoolSystem final {
	public:

		static void Create(Context*);
		static void Destroy(Context*);

		static std::vector<VkCommandBuffer> AllocateBuffers(Context*,
			uint32_t size,
			VkCommandBufferLevel level);

		static VkCommandBuffer AllocateBuffer(Context* context,
			VkCommandBufferLevel level) {
			return AllocateBuffers(context, 1, level)[0];
		}

		static void FreeBuffer(Context*, VkCommandBuffer&);
		static void ResetBuffer(VkCommandBuffer&, VkCommandBufferResetFlags);

	};
}