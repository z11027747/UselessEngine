
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	bool PhysicalDeviceLogic::Find(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &instance = global->instance;

		uint32_t tempPhysicalDeviceCount;
		vkEnumeratePhysicalDevices(instance, &tempPhysicalDeviceCount, nullptr);

		std::vector<VkPhysicalDevice> tempPhysicalDevices(tempPhysicalDeviceCount);
		vkEnumeratePhysicalDevices(instance, &tempPhysicalDeviceCount, tempPhysicalDevices.data());

		for (const auto &physicalDevice : tempPhysicalDevices)
		{
			if (!CheckType(physicalDevice,
						   VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU))
			{
				continue;
			}

			uint32_t queueFamilyIndex;
			if (!CheckQueueFamily(context,
								  physicalDevice, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT, queueFamilyIndex))
			{
				continue;
			}

			global->physicalDevice = physicalDevice;
			global->physicalQueueFamilyIndex = queueFamilyIndex;

			return true;
		}

		return false;
	}

	void PhysicalDeviceLogic::GetInfo(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		global->surfaceFormat = GetFormat(context);
		global->surfacePresentMode = GetPresentMode(context);
		global->surfaceCapabilities = GetCapbilities(context);
		global->msaaSamples = GetMaxUsableSampleCount(context);

		global->depthFormat = FindSupportedFormat(context,
												  {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM},
												  VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

		global->depthStencilFormat = FindSupportedFormat(context,
														 {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
														 VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	bool PhysicalDeviceLogic::CheckType(const VkPhysicalDevice &physicalDevice,
										VkPhysicalDeviceType deviceType)
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		return (properties.deviceType == deviceType);
	}

	bool PhysicalDeviceLogic::CheckQueueFamily(Context *context,
											   const VkPhysicalDevice &physicalDevice,
											   VkQueueFlags queueFlags,
											   uint32_t &queueFamilyIndex)
	{
		uint32_t queueFamilyPropetyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyPropeties(queueFamilyPropetyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, queueFamilyPropeties.data());

		for (auto i = 0u; i < queueFamilyPropetyCount; i++)
		{
			auto &queueFamilyPropety = queueFamilyPropeties[i];

			if (queueFamilyPropety.queueCount == 0)
			{
				continue;
			}

			if ((queueFamilyPropety.queueFlags & queueFlags) != 0)
			{
				if (!SurfaceLogic::CheckSupport(context,
												physicalDevice, i))
					continue;

				queueFamilyIndex = i;
				return true;
			}
		}

		return false;
	}

	uint32_t PhysicalDeviceLogic::FindMemoryType(Context *context,
												 uint32_t typeFilter, VkMemoryPropertyFlags propertiesFlags)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &physicalDevice = global->physicalDevice;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (auto i = 0u; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & propertiesFlags) == propertiesFlags)
			{
				return i;
			}
		}

		Common::Log::Exception("failed find memory type!");
		return -1;
	}

	VkFormat PhysicalDeviceLogic::FindSupportedFormat(Context *context,
													  const std::vector<VkFormat> &candidates,
													  VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &physicalDevice = global->physicalDevice;

		for (auto format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		Common::Log::Exception("failed to find supported format!");
		return VK_FORMAT_UNDEFINED;
	}

	VkSurfaceFormatKHR PhysicalDeviceLogic::GetFormat(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &physicalDevice = global->physicalDevice;
		auto &surface = global->surface;

		uint32_t surfaceFormatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);

		std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

		for (const auto &surfaceFormat : surfaceFormats)
		{
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
				surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return surfaceFormat;
			}
		}

		return surfaceFormats[0];
	}

	VkSurfaceCapabilitiesKHR PhysicalDeviceLogic::GetCapbilities(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &physicalDevice = global->physicalDevice;
		auto &surface = global->surface;

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

		return surfaceCapabilities;
	}

	VkPresentModeKHR PhysicalDeviceLogic::GetPresentMode(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &physicalDevice = global->physicalDevice;
		auto &surface = global->surface;

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

		auto vsync = true;
		if (!vsync)
		{
			for (auto &presentMode : presentModes)
			{
				if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
					return presentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkSampleCountFlagBits PhysicalDeviceLogic::GetMaxUsableSampleCount(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &physicalDevice = global->physicalDevice;

		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		auto counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts,
							   physicalDeviceProperties.limits.framebufferDepthSampleCounts);

		if (counts & VK_SAMPLE_COUNT_64_BIT)
			return VK_SAMPLE_COUNT_64_BIT;

		if (counts & VK_SAMPLE_COUNT_32_BIT)
			return VK_SAMPLE_COUNT_32_BIT;

		if (counts & VK_SAMPLE_COUNT_16_BIT)
			return VK_SAMPLE_COUNT_16_BIT;

		if (counts & VK_SAMPLE_COUNT_8_BIT)
			return VK_SAMPLE_COUNT_8_BIT;

		if (counts & VK_SAMPLE_COUNT_4_BIT)
			return VK_SAMPLE_COUNT_4_BIT;

		if (counts & VK_SAMPLE_COUNT_2_BIT)
			return VK_SAMPLE_COUNT_2_BIT;

		return VK_SAMPLE_COUNT_1_BIT;
	}
}