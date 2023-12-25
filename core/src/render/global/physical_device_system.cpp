
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/logical_device_system.h"
#include "render/global/physical_device_system.h"
#include "render/global/surface_system.h"
#include "context.h"

namespace Render {

	void PhysicalDeviceSystem::Create(Context* context) {

		Find(context);

		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		global->surfaceFormat = GetFormat(context);
		global->surfacePresentMode = GetPresentMode(context);
		global->surfaceCapabilities = GetCapbilities(context);
	}

	bool PhysicalDeviceSystem::Find(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& instance = global->instance;

		uint32_t tempPhysicalDeviceCount;
		vkEnumeratePhysicalDevices(instance, &tempPhysicalDeviceCount, nullptr);

		std::vector<VkPhysicalDevice> tempPhysicalDevices(tempPhysicalDeviceCount);
		vkEnumeratePhysicalDevices(instance, &tempPhysicalDeviceCount, tempPhysicalDevices.data());

		for (const auto& physicalDevice : tempPhysicalDevices) {

			if (!CheckType(physicalDevice,
				VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)) {
				continue;
			}

			uint32_t queueFamilyIndex;
			if (!CheckQueueFamily(context,
				physicalDevice, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT, queueFamilyIndex)) {
				continue;
			}

			global->physicalDevice = physicalDevice;
			global->physicalQueueFamilyIndex = queueFamilyIndex;
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
		uint32_t& queueFamilyIndex
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

				queueFamilyIndex = i;
				return true;
			}
		}

		return false;
	}

	uint32_t PhysicalDeviceSystem::FindMemoryType(Context* context,
		uint32_t typeFilter, VkMemoryPropertyFlags propertiesFlags
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& physicalDevice = global->physicalDevice;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (auto i = 0u; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i))
				&& (memProperties.memoryTypes[i].propertyFlags & propertiesFlags) == propertiesFlags) {
				return i;
			}
		}

		Common::LogSystem::Exception("failed find memory type!");
		return -1;
	}

	VkFormat PhysicalDeviceSystem::FindSupportedFormat(Context* context,
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling, VkFormatFeatureFlags features
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
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

		Common::LogSystem::Exception("failed to find supported format!");
		return VK_FORMAT_UNDEFINED;
	}


	VkSurfaceFormatKHR PhysicalDeviceSystem::GetFormat(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& physicalDevice = global->physicalDevice;
		auto& surface = global->surface;

		uint32_t surfaceFormatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);

		std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

		for (const auto& surfaceFormat : surfaceFormats) {
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM
				&& surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return surfaceFormat;
			}
		}

		return surfaceFormats[0];
	}

	VkSurfaceCapabilitiesKHR PhysicalDeviceSystem::GetCapbilities(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& physicalDevice = global->physicalDevice;
		auto& surface = global->surface;

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

		return surfaceCapabilities;
	}

	VkPresentModeKHR PhysicalDeviceSystem::GetPresentMode(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& physicalDevice = global->physicalDevice;
		auto& surface = global->surface;

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

		for (auto& presentMode : presentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return presentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}
}