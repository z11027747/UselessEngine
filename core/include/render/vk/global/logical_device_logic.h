#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class LogicalDeviceLogic final {
	public:

		static void Create(Context*);
		static void Destroy(Context*);

		static VkDevice& Get(Context*);
		static void WaitIdle(Context*);

	};

}
