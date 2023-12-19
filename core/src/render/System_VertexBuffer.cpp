
#include "render/comp.h"
#include "context.h"

//host内存 （对CPU友好，对GPU不友好）
void RenderSystem::CreateVertexBufferHost(Context* context) {
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

	//分配内存
	CreateBuffer(context,
		//buffer
		vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		//memory
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT	//GPU内存
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,//具有一致性，CPU设置完GPU就知道
		//ret
		vertexBuffer, vertexBufferMemory);

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

//device内存
//使用CPU可见的缓冲作为临时缓冲，使用显卡读取较快的缓冲作为真正的顶点缓冲
void RenderSystem::CreateVertexBufferStageing(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	//交叉顶点属性(interleaving vertex attributes)
	auto& vertices = globalInfoComp->vertices;

	vertices.push_back({ {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} });
	vertices.push_back({ {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} });
	vertices.push_back({ {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} });

	VkBuffer vertexStagingBuffer;
	VkDeviceMemory vertexStagingBufferMemory;

	VkDeviceSize vertexSize = sizeof(vertices[0]) * vertices.size();

	//分配暂存缓冲内存
	CreateBuffer(context,
		//buffer
		vertexSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, //可以被用作内存传输操作的数据来源
		//memory
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		//return
		vertexStagingBuffer, vertexStagingBufferMemory);

	void* data;
	vkMapMemory(logicDevice, vertexStagingBufferMemory, 0, vertexSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)vertexSize);
	vkUnmapMemory(logicDevice, vertexStagingBufferMemory);

	auto& vertexBuffer = globalInfoComp->vertexBuffer;
	auto& vertexBufferMemory = globalInfoComp->vertexBufferMemory;

	//分配GPULocal缓冲内存
	CreateBuffer(context,
		//buffer
		vertexSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT // 可以被用作内存传输操作的目的
		| VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		//memory
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT //最适合显卡读取的内存类型
		| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		//return
		vertexBuffer, vertexBufferMemory);

	CopyBuffer(context,
		vertexStagingBuffer, vertexBuffer, vertexSize);

	DestroyBuffer(context,
		vertexStagingBuffer, vertexStagingBufferMemory);
}

void RenderSystem::DestroyVertexBuffer(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& vertexBuffer = globalInfoComp->vertexBuffer;
	auto& vertexBufferMemory = globalInfoComp->vertexBufferMemory;

	DestroyBuffer(context,
		vertexBuffer, vertexBufferMemory);
}

