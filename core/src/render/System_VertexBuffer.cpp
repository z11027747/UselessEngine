
#include "render/comp.h"
#include "context.h"



void RenderSystem::CreateVertexBuffer(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	//交叉顶点属性(interleaving vertex attributes)
	auto& vertices = globalInfoComp->vertices;

	vertices.push_back({ {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} });
	vertices.push_back({ {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} });
	vertices.push_back({ {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} });

	auto& vertexBuffer = globalInfoComp->vertexBuffer;
	auto& vertexBufferMemory = globalInfoComp->vertexBufferMemory;

	VkDeviceSize vertexSize = sizeof(vertices[0]) * vertices.size();

	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

	//用于配置缓冲的内存稀疏程度
	//createInfo.flags = 

	createInfo.size = vertexSize; //指定要创建的缓冲所占字节大小
	createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; //缓冲中的数据的使用目的
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	auto ret = vkCreateBuffer(logicDevice, &createInfo, nullptr, &vertexBuffer);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create vertexBuffer error!");
	}

	//分配内存
	AllocateBufferMemoryType(context, vertexBuffer, vertexBufferMemory,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT		//GPU内存
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT); //具有一致性，CPU设置完GPU就知道

	//绑定内存
	auto bindRet = vkBindBufferMemory(logicDevice, vertexBuffer, vertexBufferMemory, 0);
	if (bindRet != VK_SUCCESS) {
		throw std::runtime_error("bind vertexMemory error!");
	}

	//填充内存
	void* data;

	//vkMapMemory 通过给定的内存偏移值和内存大小访问特定的内存资源
	vkMapMemory(logicDevice, vertexBufferMemory, 0, vertexSize, 0, &data);
	//memcpy将顶点数据复制到映射后的内存
	memcpy(data, vertices.data(), (size_t)vertexSize);
	//vkUnmapMemory结束内存映射
	vkUnmapMemory(logicDevice, vertexBufferMemory);

	//之前是 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 的内存，所以可以自动刷新
	//	否则需要手动
}

void RenderSystem::DestroyVertexBuffer(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& vertexBuffer = globalInfoComp->vertexBuffer;
	auto& vertexBufferMemory = globalInfoComp->vertexBufferMemory;

	vkDestroyBuffer(logicDevice, vertexBuffer, nullptr);

	FreeBufferMemoryType(context,
		vertexBufferMemory);
}

