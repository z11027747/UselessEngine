#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "base.h"

class Context;

namespace Render {

	class InstanceSystem final {
	public:

		//创建vk实例
		static void Create(Context*,
			std::vector<const char*> windowExtensions,
			bool enabledDebug
		);

		//销毁vk实例
		static void Destroy(Context*);

		//获取vk支持的版本
		static uint32_t GetApiVersion();

		//检测vk实例级的扩展
		static bool CheckExtension(const char*);

		//检测vk实例级的层
		static bool CheckLayer(const char*);

	};
}