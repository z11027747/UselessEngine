#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "render/comp.h"
#include "render/system.h"
#include "context.h"

//选取physicalDevice后，需要创建逻辑设备，之后的操作都是基于逻辑设备的
void RenderSystem::CreateLogicDevice(Context* context) {

	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& physicalDevice = globalInfo.physicalDevice;
	auto physicalDeviceGraphicsFamily = globalInfo.physicalDeviceGraphicsFamily;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	//指定创建的设备队列信息
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.queueFamilyIndex = globalInfo.physicalDeviceGraphicsFamily;
	float queuePrioprity = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePrioprity;

	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

	std::vector<const char*> deviceExtensions = {
		"VK_KHR_swapchain"
	};
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

	deviceCreateInfo.enabledLayerCount = 0;

	auto ret = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &globalInfo.logicDevice);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create device error");
	}
}

//获取创建的设备队列
void RenderSystem::GetLogicDeviceQueue(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo.logicDevice;
	auto physicalDeviceGraphicsFamily = globalInfo.physicalDeviceGraphicsFamily;

	vkGetDeviceQueue(logicDevice, physicalDeviceGraphicsFamily, 0, &globalInfo.logicQueue);
}

void RenderSystem::DestroyLogicDevice(Context* context) {
	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& logicDevice = globalInfo.logicDevice;

	vkDestroyDevice(logicDevice, nullptr);
}