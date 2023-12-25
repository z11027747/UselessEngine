
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/logical_device_system.h"
#include "render/global/physical_device_system.h"
#include "context.h"

namespace Render {

	void LogicalDeviceSystem::Create(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& physicalDevice = global->physicalDevice;
		auto physicalQueueFamilyIndex = global->physicalQueueFamilyIndex;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.queueFamilyIndex = physicalQueueFamilyIndex;
		float queuePrioprity = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePrioprity;

		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

		const std::vector<const char*> deviceExtensions = {
			"VK_KHR_swapchain"
		};
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

		deviceCreateInfo.enabledLayerCount = 0;

		VkDevice logicalDevice;
		auto ret = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
		CheckRet(ret, "vkCreateDevice");

		VkQueue logicalQueue;
		vkGetDeviceQueue(logicalDevice, physicalQueueFamilyIndex, 0, &logicalQueue);

		global->logicalDevice = logicalDevice;
		global->logicalQueue = logicalQueue;
	}

	void LogicalDeviceSystem::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		vkDestroyDevice(logicalDevice, nullptr);
	}

	void LogicalDeviceSystem::WaitIdle(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		vkDeviceWaitIdle(logicalDevice);
	}

}