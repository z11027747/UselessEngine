
#include "render/global/global_def.h"
#include "render/device/device_comp.h"
#include "render/device/logical_device_system.h"
#include "render/device/physical_device_system.h"
#include "render/instance/instance_comp.h"
#include "render/instance/instance_system.h"
#include "context.h"

namespace Render {


	void LogicalDeviceSystem::Create(Context* context) {

		VkPhysicalDevice physicalDevice;
		uint32_t physicalQueueFamilyIndex;

		PhysicalDeviceSystem::Find(context,
			physicalDevice,
			physicalQueueFamilyIndex);

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		//指定创建的设备队列信息
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

		auto device = std::make_shared<Render::Device>();
		device->physicalDevice = physicalDevice;
		device->physicalQueueFamilyIndex = physicalQueueFamilyIndex;
		device->logicalDevice = logicalDevice;
		device->logicalQueue = logicalQueue;

		auto& renderGlobalEO = context->renderGlobalEO;
		renderGlobalEO->AddComponent<Render::Device>(device);
	}

	void LogicalDeviceSystem::Destroy(Context* context) {

		auto& renderGlobalEO = context->renderGlobalEO;
		auto device = renderGlobalEO->GetComponent<Render::Device>();

		vkDestroyDevice(device->logicalDevice, nullptr);

		renderGlobalEO->RemoveComponent<Render::Device>();
	}

}