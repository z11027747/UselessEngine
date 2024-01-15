
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{

	void LogicalDeviceLogic::Create(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &physicalDevice = global->physicalDevice;
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

		const std::vector<const char *> deviceExtensions = {
			"VK_KHR_swapchain"};
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

		deviceCreateInfo.enabledLayerCount = 0;

		VkPhysicalDeviceFeatures feature = {};
		feature.fillModeNonSolid = VK_TRUE;
		feature.wideLines = true;
		deviceCreateInfo.pEnabledFeatures = &feature;

		VkDevice logicalDevice;
		auto ret = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
		CheckRet(ret, "vkCreateDevice");

		VkQueue logicalQueue;
		vkGetDeviceQueue(logicalDevice, physicalQueueFamilyIndex, 0, &logicalQueue);

		global->logicalDevice = logicalDevice;
		global->logicalQueue = logicalQueue;
	}

	void LogicalDeviceLogic::Destroy(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		vkDestroyDevice(logicalDevice, nullptr);
	}

	VkDevice &LogicalDeviceLogic::Get(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		return logicalDevice;
	}

	void LogicalDeviceLogic::WaitIdle(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		vkDeviceWaitIdle(logicalDevice);
	}

}