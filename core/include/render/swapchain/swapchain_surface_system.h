#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/swapchain/swapchain_comp.h"

class Context;

namespace Render {

	class SwapchainSurfaceSystem final {
	public:

		//创建表面窗口
		static void Create(Context*,
			std::shared_ptr<Swapchain> swapchain
		);

		//销毁表面窗口
		static void Destroy(Context*,
			std::shared_ptr<Swapchain> swapchain
		);

		//检测队列族是否支持表面窗口
		static bool CheckSupport(Context*,
			const VkPhysicalDevice& physicalDevice,
			uint32_t queueFamilyIndex
		);

		//获取表面支持的格式
		static VkSurfaceFormatKHR GetFormat(Context*);
		//获取表面可用的呈现模式
		static VkPresentModeKHR GetPresentMode(Context*);
		//获取表面支持的特性
		static VkSurfaceCapabilitiesKHR GetCapbilities(Context*);


	};


}