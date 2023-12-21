#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render {

	class CmdPoolSystem final {
	public:

		//创建指令池
		static void Create(Context*);

		//销毁指令池
		static void Destroy(Context*);

		//分配多个指令缓冲对象
		static std::vector<VkCommandBuffer> AllocateBuffers(Context*,
			uint32_t size,
			VkCommandBufferLevel level);

		//分配一个指令缓冲对象
		static VkCommandBuffer AllocateBuffer(Context* context,
			VkCommandBufferLevel level) {
			return AllocateBuffers(context, 1, level)[0];
		}

		//回收一个指令缓冲对象
		static void FreeBuffer(Context*, VkCommandBuffer&);

		//重置一个指令缓冲对象
		static void ResetBuffer(VkCommandBuffer&, VkCommandBufferResetFlags);

	};
}