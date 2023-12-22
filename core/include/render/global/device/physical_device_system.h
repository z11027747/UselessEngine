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
		static bool CheckQueueFamily(Context*,
			const VkPhysicalDevice& physicalDevice,
			VkQueueFlags queueFlags,
			uint32_t& queueFamily);

		//选择内存类型
		static uint32_t FindMemoryType(Context*,
			uint32_t typeFilter,
			VkMemoryPropertyFlags propertiesFlags);

		//选择支持的格式
		static VkFormat FindSupportedFormat(Context*,
			const std::vector<VkFormat>& candidates,
			VkImageTiling tiling, VkFormatFeatureFlags features
		);
	};

}
