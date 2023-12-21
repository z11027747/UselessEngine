#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render {

	struct CmdPool final {
		VkCommandPool vkPool;
	};

	struct CmdSubmit final {
		VkCommandBuffer vkBuffer;
	};

	struct CmdSubmitSemaphore final {
		VkCommandBuffer vkBuffer;
		VkSemaphore waitSemaphore;
		VkSemaphore signalSemaphore;
	};

}