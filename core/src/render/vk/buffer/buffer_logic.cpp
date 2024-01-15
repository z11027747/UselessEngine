
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	std::shared_ptr<Buffer> BufferLogic::Create(Context *context,
												VkDeviceSize size, VkBufferUsageFlags usageFlags,
												VkMemoryPropertyFlags propertiesFlags)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &physicalDevice = global->physicalDevice;
		auto &logicalDevice = global->logicalDevice;

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = usageFlags;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer vkBuffer;
		auto bufferRet = vkCreateBuffer(logicalDevice, &createInfo, nullptr, &vkBuffer);
		CheckRet(bufferRet, "vkCreateBuffer");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, vkBuffer, &memRequirements);

		auto memoryTypeIndex = PhysicalDeviceLogic::FindMemoryType(context,
																   memRequirements.memoryTypeBits, propertiesFlags);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memRequirements.size; // 实际大小，内存对齐
		allocateInfo.memoryTypeIndex = memoryTypeIndex;

		VkDeviceMemory vkDeviceMemory;
		auto allocateRet = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &vkDeviceMemory);
		CheckRet(allocateRet, "vkAllocateMemory");

		auto bindRet = vkBindBufferMemory(logicalDevice, vkBuffer, vkDeviceMemory, 0);
		CheckRet(bindRet, "vkBindBufferMemory");

		auto buffer = std::make_shared<Buffer>();
		buffer->vkBuffer = vkBuffer;
		buffer->vkDeviceMemory = vkDeviceMemory;
		buffer->size = memRequirements.size;
		return buffer;
	}

	void BufferLogic::Destroy(Context *context,
							  std::shared_ptr<Buffer> buffer)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;

		vkDestroyBuffer(logicalDevice, buffer->vkBuffer, nullptr);
		vkFreeMemory(logicalDevice, buffer->vkDeviceMemory, nullptr);
	}

	std::shared_ptr<Buffer> BufferLogic::CreateTemp(Context *context,
													VkDeviceSize size, VkBufferUsageFlags usageFlags,
													VkMemoryPropertyFlags propertiesFlags)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto buffer = Create(context,
							 size, usageFlags,
							 propertiesFlags);

		global->tempBuffers.push_back(buffer);
		return buffer;
	}

	void BufferLogic::DestroyAllTemps(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto &tempBuffers = global->tempBuffers;
		for (auto &tempBuffer : tempBuffers)
		{
			Destroy(context,
					tempBuffer);
		}
		tempBuffers.clear();
	}
}