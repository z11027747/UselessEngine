#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

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
		static VkCommandBuffer CreateAndBegin(Context *);
		static void End(Context *, VkCommandBuffer &);
		static void EndSingleTime(Context *, VkCommandBuffer &);
		static void BatchAll(Context *);
	};
}