#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <functional>
#include "render/vk/cmd/cmd_comp.h"

class Context;

namespace Render
{
	class CmdPoolLogic final
	{
	public:
		static void Create(Context *);
		static void Destroy(Context *);

		static std::vector<VkCommandBuffer> CreateBuffers(Context *, uint32_t);
		static VkCommandBuffer CreateBuffer(Context *);

		static VkCommandBuffer CreateTempBuffer(Context *);
		static void DestroyAllTempBuffers(Context *);

		static void FreeBuffers(Context *, std::vector<VkCommandBuffer> &);
		static void ResetBuffer(VkCommandBuffer &, VkCommandBufferResetFlags);
	};

	class CmdSubmitLogic final
	{
	public:
		static void Create(Context *, std::function<void(VkCommandBuffer &)> doCmds);
		static void Record(VkCommandBuffer &, std::function<void(VkCommandBuffer &)> doCmds);

		static VkCommandBuffer CreateAndBegin(Context *);
		static void End(Context *, VkCommandBuffer &);

		static void UpdateBatch(Context *);
	};
}