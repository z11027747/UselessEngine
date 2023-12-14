#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "render/comp.h"
#include "render/system.h"
#include "context.h"

//创建instance之后，可以按照需要选择任意的物理设备
void RenderSystem::PickupPhysicalDevice(Context* context) {

	auto& renderEO = context->renderEO;
	auto& globalInfo = renderEO.GetComp<RenderGlobal>(RenderType::eGlobalInfo);

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

		uint32_t queueFamilyPropetyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyPropeties(queueFamilyPropetyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, queueFamilyPropeties.data());

		for (int i = 0; i < queueFamilyPropetyCount; i++) {
			auto& queueFamilyPropety = queueFamilyPropeties[i];

			if (queueFamilyPropety.queueCount > 0
				&& queueFamilyPropety.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
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
