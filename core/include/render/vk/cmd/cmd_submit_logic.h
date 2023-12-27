#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <functional>

class Context;

namespace Render {

	class CmdSubmitLogic final {
	public:

		static void Create(Context*,
			std::function<void(VkCommandBuffer&)> doCmds);

		static void Record(VkCommandBuffer&,
			std::function<void(VkCommandBuffer&)> doCmds
		);

		static void Update(Context*);

	};

}