#pragma once

#include <vulkan/vulkan.h>
#include "render/vk/image/image_comp.h"

class Context;

namespace Render
{
	class SamplerLogic final
	{
	public:
		static VkSampler Create(Context *, bool,bool, uint32_t = 0, uint32_t = 1);
		static VkSampler CreateDepth(Context *);
		static void Destroy(Context *, VkSampler &);
	};
}