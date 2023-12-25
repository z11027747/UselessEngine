#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class LogicalDeviceSystem final {
	public:

		static void Create(Context*);
		static void Destroy(Context*);

		static void WaitIdle(Context*);

	};

}
