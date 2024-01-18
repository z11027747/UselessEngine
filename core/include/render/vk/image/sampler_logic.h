#pragma once

#include <vulkan/vulkan.h>
#include "render/vk/image/image_comp.h"

class Context;

namespace Render
{
	class SamplerLogic final
	{
	public:
		static VkSampler Create(Context *, bool, bool forDepth = false);
		static void Destroy(Context *, VkSampler &);
	};
}