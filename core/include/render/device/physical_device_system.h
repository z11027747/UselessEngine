#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class PhysicalDeviceSystem final {
	public:

		//创建instance之后，按需选择物理设备
		static bool Find(Context*,
			VkPhysicalDevice& physicalDevice,
			uint32_t& queueFamily);

		//检查物理显卡类型
		static bool CheckType(
			const VkPhysicalDevice& physicalDevice,
			VkPhysicalDeviceType deviceType);

		//检查物理显卡队列族
		static bool CheckQueueFamily(
			const VkPhysicalDevice& physicalDevice,
			VkQueueFlags queueFlags,
			uint32_t& queueFamily);

		//选择内存类型
		static bool FindMemoryType(Context*,
			uint32_t typeFilter,
			VkMemoryPropertyFlags propertiesFlags);

	};

}
