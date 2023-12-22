
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/base/buffer/buffer_comp.h"
#include "render/base/buffer/buffer_system.h"
#include "context.h"

namespace Render {

	std::shared_ptr<Buffer> BufferSystem::Create(Context* context,
		VkDeviceSize size, VkBufferUsageFlags usageFlags,
		VkMemoryPropertyFlags propertiesFlags
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& physicalDevice = global->physicalDevice;
		auto& logicalDevice = global->logicalDevice;

		//1.创建 vkBuffer
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//createInfo.flags = 0; //用于配置缓冲的内存稀疏程度
		createInfo.size = size; //指定要创建的缓冲所占字节大小
		createInfo.usage = usageFlags; //缓冲中的数据的使用目的
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer vkBuffer;
		auto bufferRet = vkCreateBuffer(logicalDevice, &createInfo, nullptr, &vkBuffer);
		CheckRet(bufferRet, "vkCreateBuffer");

		//2.获取内存需求，分配内存
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, vkBuffer, &memRequirements);

		auto memoryTypeIndex = PhysicalDeviceSystem::FindMemoryType(context,
			memRequirements.memoryTypeBits, propertiesFlags);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memRequirements.size; //实际大小，内存对齐
		allocateInfo.memoryTypeIndex = memoryTypeIndex;

		VkDeviceMemory vkDeviceMemory;
		auto allocateRet = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &vkDeviceMemory);
		CheckRet(allocateRet, "vkAllocateMemory");

		//3.绑定内存
		auto bindRet = vkBindBufferMemory(logicalDevice, vkBuffer, vkDeviceMemory, 0);
		CheckRet(bindRet, "vkBindBufferMemory");

		auto buffer = std::make_shared<Buffer>();
		buffer->vkBuffer = vkBuffer;
		buffer->vkDeviceMemory = vkDeviceMemory;
		return buffer;
	}

	void BufferSystem::Destroy(Context* context,
		std::shared_ptr<Buffer> buffer
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyBuffer(logicalDevice, buffer->vkBuffer, nullptr);
		vkFreeMemory(logicalDevice, buffer->vkDeviceMemory, nullptr);
	}

}