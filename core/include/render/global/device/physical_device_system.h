#pragma once

#include <vulkan/vulkan.h>

class Context;

namespace Render {

	class PhysicalDeviceSystem final {
	public:

		static void Create(Context*);
		static bool Find(Context*);

		static bool CheckType(
			const VkPhysicalDevice& physicalDevice,
			VkPhysicalDeviceType deviceType);

		static bool CheckQueueFamily(Context*,
			const VkPhysicalDevice& physicalDevice,
			VkQueueFlags queueFlags,
			uint32_t& queueFamilyIndex);

		static uint32_t FindMemoryType(Context*,
			uint32_t typeFilter,
			VkMemoryPropertyFlags propertiesFlags);

		static VkFormat FindSupportedFormat(Context*,
			const std::vector<VkFormat>& candidates,
			VkImageTiling tiling, VkFormatFeatureFlags features
		);

		static VkSurfaceFormatKHR GetFormat(Context*);
		static VkSurfaceCapabilitiesKHR GetCapbilities(Context*);
		static VkPresentModeKHR GetPresentMode(Context*);
	};

}
