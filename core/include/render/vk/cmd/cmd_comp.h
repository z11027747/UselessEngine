#pragma once

#include <vulkan/vulkan.h>
#include <vector>

class Context;

namespace Render
{
	struct Cmd final
	{
		std::vector<VkCommandBuffer> vkCmdBuffers;
	};
}