
#include "render/global/global_def.h"
#include "render/device/device_comp.h"
#include "render/device/physical_device_system.h"
#include "render/instance/instance_comp.h"
#include "render/instance/instance_system.h"
#include "context.h"

namespace Render {

	//创建instance之后，可以按照需要选择任意的物理设备
	bool PhysicalDeviceSystem::Find(Context* context,
		VkPhysicalDevice& physicalDevice,
		uint32_t& queueFamily
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto instance = renderGlobalEO->GetComponent<Instance>();
		auto& vkInstance = instance->vkInstance;

		uint32_t tempPhysicalDeviceCount;
		vkEnumeratePhysicalDevices(vkInstance, &tempPhysicalDeviceCount, nullptr);

		std::vector<VkPhysicalDevice> tempPhysicalDevices(tempPhysicalDeviceCount);
		vkEnumeratePhysicalDevices(vkInstance, &tempPhysicalDeviceCount, tempPhysicalDevices.data());

		for (const auto& tempPhysicalDevice : tempPhysicalDevices) {

			//需要独立显卡
			if (!CheckType(tempPhysicalDevice,
				VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)) {
				continue;
			}

			//需要图形&传输队列族
			if (!CheckQueueFamily(tempPhysicalDevice,
				VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT, queueFamily)) {
				continue;
			}

			physicalDevice = tempPhysicalDevice;
			return true;
		}

		return false;
	}

	//VkPhysicalDeviceType 显卡类型
	//	VK_PHYSICAL_DEVICE_TYPE_OTHER 该设备类型不与任何其他类型匹配， Vulkan 中未定义的设备类型
	//	VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU 集成显卡
	//	VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 独立显卡
	//	VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU 虚拟环境中的虚拟显卡
	bool PhysicalDeviceSystem::CheckType(
		const VkPhysicalDevice& physicalDevice,
		VkPhysicalDeviceType deviceType
	) {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		return (properties.deviceType == deviceType);

	}

	//队列族
	//vulkan的几乎所有操作都需要将操作指令提交给一个队列，然后执行
	//Vulkan有多种不同类型的队列，它们属于不同的队列族，每个队列族的队列只允许执行特定的一部分指令
	bool PhysicalDeviceSystem::CheckQueueFamily(
		const VkPhysicalDevice& physicalDevice,
		VkQueueFlags queueFlags,
		uint32_t& queueFamily
	) {
		uint32_t queueFamilyPropetyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyPropeties(queueFamilyPropetyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropetyCount, queueFamilyPropeties.data());

		for (uint32_t i = 0; i < queueFamilyPropetyCount; i++) {
			auto& queueFamilyPropety = queueFamilyPropeties[i];

			//每个队列族支持1个-多个类型
			if (queueFamilyPropety.queueCount == 0) {
				continue;
			}

			//queueFlags 队列类型
			//VK_QUEUE_GRAPHICS_BIT 主要用于图形渲染，执行各种渲染绘制指令
			//VK_QUEUE_COMPUTE_BIT 主要用于执行并行计算（计算着色器），执行各种计算指令
			//VK_QUEUE_TRANSFER_BIT 主要用于执行资源的布局转移并支持在不同队列中进行转移，执行各种转移指令
			if ((queueFamilyPropety.queueFlags & queueFlags) != 0) {

				//支持呈现图像到窗口表面能力的队列族
				//TODO

				queueFamily = i;
				return true;
			}
		}

		return false;
	}


	//显卡可以分配不同类型的内存作为缓冲使用。
	//	不同类型的内存所允许进行的操作以及操作的效率有所不同
	//	我们需要结合自己的需求选择最合适的内存类型使用。
	bool PhysicalDeviceSystem::FindMemoryType(Context* context,
		uint32_t typeFilter, VkMemoryPropertyFlags propertiesFlags
	) {
		auto& renderEO = context->renderEO;

		auto device = renderEO->GetComponent<Device>();
		auto& physicalDevice = device->physicalDevice;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (auto i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i))
				&& (memProperties.memoryTypes[i].propertyFlags & propertiesFlags) == propertiesFlags) {
				return i;
			}
		}

		throw std::runtime_error("failed find memory type!");
	}
}