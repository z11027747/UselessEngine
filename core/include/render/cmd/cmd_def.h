#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "base.h"

class Context;

namespace Render {

	struct CmdPool final {
		VkCommandPool commandPool;
	};

	struct CmdSimple final {
		VkCommandBuffer commandBuffer;
	};

	struct CmdWithSephores final {
		VkCommandBuffer commandBuffer;
		VkSemaphore waitSemaphore;
		VkSemaphore signalSemaphore;
	};

	class CmdSystem final {
	public:

		//创建指令池
		static void CreatePool(Context*);




	};
}