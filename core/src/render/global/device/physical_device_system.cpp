
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/global/surface/surface_system.h"
#include "context.h"

namespace Render {

	bool PhysicalDeviceSystem::Find(Context* context,
		VkPhysicalDevice& physicalDevice,
		uint32_t& queueFamily
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& instance = global->instance;

		uint32_t tempPhysicalDeviceCount;
		vkEnumeratePhysicalDevices(instance, &tempPhysicalDeviceCount, nullptr);

		std::vector<VkPhysicalDevice> tempPhysicalDevices(tempPhysicalDeviceCount);
		vkEnumeratePhysicalDevices(instance, &tempPhysicalDeviceCount, tempPhysicalDevices.data());

		for (const auto& tempPhysicalDevice : tempPhysicalDevices) {

			if (!CheckType(tempPhysicalDevice,
				VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)) {
				continue;
			}

			if (!CheckQueueFamily(context,
				tempPhysicalDevice, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT, queueFamily)) {
				continue;
			}

			physicalDevice = tempPhysicalDevice;
			return true;
		}

		return false;
	}

	bool PhysicalDeviceSystem::CheckType(
		const VkPhysicalDevice& physicalDevice,
		VkPhysicalDeviceType deviceType
	) {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		return (properties.deviceType == deviceType);
	}

	bool PhysicalDeviceSystem::CheckQueueFamily(Context* context,
		const VkPhysicalDevice& physicalDevice,
		VkQueueFlags queueFlags,
		uint32_t& queueFamily
	) {
		uint32_t queueFamilyPropetyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyPropeties(queueFamilyPropetyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, queueFamilyPropeties.data());

		for (auto i = 0u; i < queueFamilyPropetyCount; i++) {
			auto& queueFamilyPropety = queueFamilyPropeties[i];

			if (queueFamilyPropety.queueCount == 0) {
				continue;
			}

			if ((queueFamilyPropety.queueFlags & queueFlags) != 0) {

				if (!SurfaceSystem::CheckSupport(context,
					physicalDevice, i))
					continue;

				queueFamily = i;
				return true;
			}
		}

		return false;
	}

	uint32_t PhysicalDeviceSystem::FindMemoryType(Context* context,
		uint32_t typeFilter, VkMemoryPropertyFlags propertiesFlags
	) {
		auto& renderEO = context->renderEO;

		auto global = renderEO->GetComponent<Global>();
		auto& physicalDevice = global->physicalDevice;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (auto i = 0u; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i))
				&& (memProperties.memoryTypes[i].propertyFlags & propertiesFlags) == propertiesFlags) {
				return i;
			}
		}

		Log::System::Exception("failed find memory type!");
		return -1;
	}

	VkFormat PhysicalDeviceSystem::FindSupportedFormat(Context* context,
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling, VkFormatFeatureFlags features
	) {
		auto& renderEO = context->renderEO;

		auto global = renderEO->GetComponent<Global>();
		auto& physicalDevice = global->physicalDevice;

		for (auto format : candidates) {

			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR
				&& (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL
				&& (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		Log::System::Exception("failed to find supported format!");
		return VK_FORMAT_UNDEFINED;
	}
}