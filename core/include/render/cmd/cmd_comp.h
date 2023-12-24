#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render {

	struct CmdPool final {
		VkCommandPool vkPool;
	};

	struct CmdSimple final {
		std::vector<VkCommandBuffer> vkCmdBuffers;
	};

	struct CmdSubmitSemaphore final {
		std::vector<VkCommandBuffer> vkCmdBuffers;
		VkSemaphore waitSemaphore;
		VkSemaphore signalSemaphore;
	};

}