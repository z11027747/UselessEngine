
#include "render/comp.h"
#include "context.h"

//内存
//Instance域函数中主要在 CPU 能够访问的（主板上）内存中进行分配和访问
//		该部分内存属于特殊的 Host 端内存
//Deivce域函数中主要在 GPU 能够访问的内存（显存）中进行内存分配和访问 
//		该部分内存属于特殊的 Device 端内存

//cpu 创建 host内存（使用new）
//拷贝到 host和device 都可以访问的内存 （对CPU友好，对GPU不友好）
//通过总线拷贝到 只有device 可以访问的内存

//VkPhysicalDeviceMemoryProperties  内存信息
//	memoryTypeCount 内存类型的数量
//	memoryTypes 对应的内存类型信息数据
//	memoryHeapCount 内存堆的数量
//	memoryHeaps 对应的内存堆的信息数据

//VkMemoryType	 内存类型
//	propertyFlags 该类内存的属性信息，使用标志位存储相应信息
//	heapIndex 对应的内存堆的索引，表示指向 memoryHeaps[heapIndex] 的内存堆

//VkMemoryPropertyFlagBits  内存标志位
//	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT 表示这部分内存为 GPU 物理设备自身的内存只有物理设备自身可访问，也就是 Device 端内存。
//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 表示这部分内存为 Host 端可访问到的内存只有 Host 端自身可访问， Device 端不可访问。
//	VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 表示这部分内存为 Host 端连续内存，表示对于该内存的读写可连续进行（就像 CPU 对于内存的修改那样）。该内存类型不需要手动进行 刷新 和 失效 操作。
//	VK_MEMORY_PROPERTY_HOST_CACHED_BIT 表示这部分内存为 Host 端高速内存，并且自带 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 属性。这一部分内存大小相对较小。
//	VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT 表示这部分内存为可以滞后分配内存，等要使用时再分配内存。
//	VK_MEMORY_PROPERTY_PROTECTED_BIT 表示这部分内存为受保护内存，并且只允许 GPU 硬件设备和受保护的队列（ VK_QUEUE_PROTECTED_BIT ）可以访问该内存。


//VkMemoryHeap	内存堆
//	size 表示该内存堆的大小。单位为字节
//	flags 表示该堆的属性标志位

//VkMemoryHeapFlags              
//	VK_MEMORY_HEAP_DEVICE_LOCAL_BIT 表示该内存堆为 GPU 专属内存
//	VK_MEMORY_HEAP_MULTI_INSTANCE_BIT 由于逻辑设备可以包含多个物理设备，此标志位表示该堆对应多个物理设备上的内存堆，对该堆的操作将会在每个物理设备的内存堆上进行相同的操作。

void RenderSystem::CheckPhysicalDeviceMemory(Context* context) {

	auto& renderEO = context->renderEO;

	auto globalInfo = renderEO->GetComponent<RenderGlobal>();
	auto& physicalDevice = globalInfo->physicalDevice;

	VkPhysicalDeviceMemoryProperties memoryProerties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProerties);

	for (uint32_t i = 0; i < memoryProerties.memoryTypeCount; i++) {
		auto memoryType = memoryProerties.memoryTypes[i];

		std::cout << "=== >index: " << i << std::endl;
		if (memoryType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			std::cout << "Supprt DEVICE_LOCAL" << std::endl;
		}
		if (memoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
			std::cout << "Supprt HOST_VISIBLE" << std::endl;
		}
		if (memoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
			std::cout << "Supprt HOST_COHERENT" << std::endl;
		}

		auto heapIndex = memoryType.heapIndex;
		auto memoryHeap = memoryProerties.memoryHeaps[heapIndex];

		std::cout << "heap index: " << heapIndex << ":" << (memoryHeap.size) / (1024 * 1024 * 1024) << " GB" << std::endl;
		if (memoryHeap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
			std::cout << "\t DEVICE_LOCAL" << std::endl;
		}
	}
}


//分配内存使用 vkAllocateMemory
//回收内存使用 vkFreeMemory 