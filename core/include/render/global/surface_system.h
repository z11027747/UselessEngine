#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/global/global_comp.h"

class Context;

namespace Render {

	class SurfaceSystem final {
	public:

		static void Create(Context*);
		static void Destroy(Context*);

		static bool CheckSupport(Context*,
			const VkPhysicalDevice& physicalDevice,
			uint32_t queueFamilyIndex
		);

	};


}