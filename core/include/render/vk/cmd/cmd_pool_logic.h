#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
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
}