#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "render/comp.h"
#include "render/system.h"
#include "context.h"

//创建instance之后，可以按照需要选择任意的物理设备
void RenderSystem::PickupPhysicalDevice(Context* context) {

	auto& renderEO = context->renderEO;

	auto& globalInfo = renderEO.GetComponent<RenderGlobal>();
	auto& instance = globalInfo.instance;

	globalInfo.physicalDevice = nullptr;
	globalInfo.physicalDeviceGraphicsFamily = -1;

	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	for (const auto& physicalDevice : physicalDevices) {

		//队列族
		//vulkan的几乎所有操作都需要将操作指令提交给一个队列，然后执行
		//Vulkan有多种不同类型的队列，它们属于不同的队列族，每个队列族的队列只允许执行特定的一部分指令

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		//VkPhysicalDeviceType 显卡类型
		//	VK_PHYSICAL_DEVICE_TYPE_OTHER 该设备类型不与任何其他类型匹配， Vulkan 中未定义的设备类型
		//	VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU 集成显卡
		//	VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 独立显卡
		//	VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU 虚拟环境中的虚拟显卡
		if (properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			continue;
		}

		uint32_t queueFamilyPropetyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyPropeties(queueFamilyPropetyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, queueFamilyPropeties.data());

		for (int i = 0; i < queueFamilyPropetyCount; i++) {
			auto& queueFamilyPropety = queueFamilyPropeties[i];

			//每个队列族支持1个-多个类型
			if (queueFamilyPropety.queueCount == 0) {
				continue;
			}

			//queueFlags 队列类型
			//VK_QUEUE_GRAPHICS_BIT 主要用于图形渲染，执行各种渲染绘制指令
			//VK_QUEUE_COMPUTE_BIT 主要用于执行并行计算（计算着色器），执行各种计算指令
			//VK_QUEUE_TRANSFER_BIT 主要用于执行资源的布局转移并支持在不同队列中进行转移，执行各种转移指令

			if (queueFamilyPropety.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				//优先找支持图形功能的队列族
				//	大部分设备队列族如果支持图形功能的话，其他的计算、转移和稀疏绑定功能也会同时支持
				globalInfo.physicalDevice = physicalDevice;
				globalInfo.physicalDeviceGraphicsFamily = i;
				break;
			}
		}

		if (globalInfo.physicalDeviceGraphicsFamily != -1) {
			break;
		}
	}
}


