#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <functional>
#include "render/vk/cmd/cmd_comp.h"

class Context;

namespace Render {

	class CmdPoolLogic final {
	public:
		static void Create(Context*);
		static void Destroy(Context*);

		static std::shared_ptr<Cmd> CreateCmd(Context*);

		static void Allocate(Context*, 	std::shared_ptr<Cmd>, uint32_t);
		static void Free(Context*, std::shared_ptr<Cmd>);

		static std::shared_ptr<Cmd> CreateTempCmd(Context*);
		static void DestroyAllTemps(Context*);

		static void ResetBuffer(VkCommandBuffer&, VkCommandBufferResetFlags);
	};

	class CmdSubmitLogic final {
	public:
		static void Create(Context*, std::function<void(VkCommandBuffer&)> doCmds);

		static void Record(VkCommandBuffer&, std::function<void(VkCommandBuffer&)> doCmds);

		static void UpdateBatch(Context*);
	};
}