
#include "render/global/global_def.h"
#include "render/device/device_comp.h"
#include "render/device/logical_device_system.h"
#include "render/device/physical_device_system.h"
#include "render/buffer/buffer_comp.h"
#include "render/buffer/buffer_system.h"
#include "context.h"

namespace Render {

	std::shared_ptr<Buffer> BufferSystem::Create(Context* context,
		VkDeviceSize size, VkBufferUsageFlags usageFlags,
		VkMemoryPropertyFlags propertiesFlags
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto device = renderGlobalEO->GetComponent<Device>();
		auto& physicalDevice = device->physicalDevice;
		auto& logicalDevice = device->logicalDevice;

		//1.���� vkBuffer
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//createInfo.flags = 0; //�������û�����ڴ�ϡ��̶�
		createInfo.size = size; //ָ��Ҫ�����Ļ�����ռ�ֽڴ�С
		createInfo.usage = usageFlags; //�����е����ݵ�ʹ��Ŀ��
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer vkBuffer;
		auto bufferRet = vkCreateBuffer(logicalDevice, &createInfo, nullptr, &vkBuffer);
		CheckRet(bufferRet, "vkCreateBuffer");

		//2.��ȡ�ڴ����󣬷����ڴ�
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, vkBuffer, &memRequirements);

		auto memoryTypeIndex = PhysicalDeviceSystem::FindMemoryType(context,
			memRequirements.memoryTypeBits, propertiesFlags);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memRequirements.size; //ʵ�ʴ�С���ڴ����
		allocateInfo.memoryTypeIndex = memoryTypeIndex;

		VkDeviceMemory vkDeviceMemory;
		auto allocateRet = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &vkDeviceMemory);
		CheckRet(allocateRet, "vkAllocateMemory");

		//3.���ڴ�
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

		auto device = renderGlobalEO->GetComponent<Device>();
		auto& logicalDevice = device->logicalDevice;

		vkDestroyBuffer(logicalDevice, buffer->vkBuffer, nullptr);
		vkFreeMemory(logicalDevice, buffer->vkDeviceMemory, nullptr);
	}

}