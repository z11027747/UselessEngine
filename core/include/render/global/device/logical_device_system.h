#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class LogicalDeviceSystem final {
	public:

		//创建逻辑设备
		static void Create(Context*);

		//销毁逻辑设备
		static void Destroy(Context*);

	};

}
