﻿
#include "render/comp.h"
#include "context.h"

//对于一些所有顶点都共享的属性，比如顶点的变换矩阵
// 将它们作为顶点属性为每个顶点都传递一份显然是非常浪费的

//Vulkan提供了资源描述符来解决这一问题
//描述符：用来在着色器中访问缓冲（Buffer）和图像数据（Image）的一种方式
//可以将变换矩阵存储在一个缓冲中，然后通过描述符在着色器中访问它

//步骤：
//	1.在管线创建时指定描述符布局
//	2.从描述符池分配描述符集
//	3.在渲染时绑定描述符集

//绑定的描述符类型的描述符布局
void RenderSystem::CreateDescriptorSetLayout(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	auto& descriptorSetLayout = globalInfoComp->descriptorSetLayout;

	VkDescriptorSetLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

	//VkDescriptorSetLayoutBinding 描述每个binding
	std::array<VkDescriptorSetLayoutBinding, 2> bindings = {};

	bindings[0].binding = 0; //index
	//描述符类型和数量
	//	意义是：1个uniform的Buffer
	bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	bindings[0].descriptorCount = 1;
	//哪个着色器阶段用
	bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	bindings[1].binding = 1;
	bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //一个包含图像和采样器的组合对象
	bindings[1].descriptorCount = 1;
	bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	createInfo.pBindings = bindings.data();

	auto ret = vkCreateDescriptorSetLayout(logicDevice, &createInfo, nullptr, &descriptorSetLayout);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create descriptorSetLayout error!");
	}
}

void RenderSystem::DestroyDescriptorSetLayout(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& descriptorSetLayout = globalInfoComp->descriptorSetLayout;
	vkDestroyDescriptorSetLayout(logicDevice, descriptorSetLayout, nullptr);
}

void RenderSystem::CreateUniformBuffersHost(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	auto& descriptorSetLayout = globalInfoComp->descriptorSetLayout;
	VkDeviceSize uniformSize = sizeof(UniformBufferObject);

	auto& swapchainImages = globalInfoComp->swapchainImages;
	auto swapchainImageSize = swapchainImages.size();

	auto& uniformBuffers = globalInfoComp->uniformBuffers;
	auto& uniformBufferMemorys = globalInfoComp->uniformBufferMemorys;

	uniformBuffers.resize(swapchainImageSize);
	uniformBufferMemorys.resize(swapchainImageSize);

	for (auto i = 0; i < swapchainImageSize; i++) {

		CreateBuffer(context,
			//buffer
			uniformSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			//memory
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			//return
			uniformBuffers[i], uniformBufferMemorys[i]);
	}

	//另外的函数中使用新的变换矩阵更新uniform缓冲
	// 这里没有出现vkMapMemory函数调用
}

void RenderSystem::DestroyUniformBuffers(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();

	auto& swapchainImages = globalInfoComp->swapchainImages;
	auto swapchainImageSize = swapchainImages.size();

	auto& uniformBuffers = globalInfoComp->uniformBuffers;
	auto& uniformBufferMemorys = globalInfoComp->uniformBufferMemorys;

	for (auto i = 0; i < swapchainImageSize; i++) {

		DestroyBuffer(context,
			uniformBuffers[i], uniformBufferMemorys[i]);
	}
}

void RenderSystem::UpdateUniformBuffer(Context* context, uint32_t index) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& swapChainExtent = globalInfoComp->swapChainExtent;

	auto& uniformBufferMemorys = globalInfoComp->uniformBufferMemorys;
	auto& uniformBufferMemory = uniformBufferMemorys[index];

	auto currTime = context->currTime;

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), currTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);

	//GLM库最初是为OpenGL设计的，它的裁剪坐标的Y轴和Vulkan是相反的
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(logicDevice, uniformBufferMemory, 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(logicDevice, uniformBufferMemory);
}

//描述符集不能被直接创建，需要通过描述符池来分配
void RenderSystem::CreateDescriptorPool(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	//每一帧分配一个描述符 对应交换链的图像数
	auto& swapchainImages = globalInfoComp->swapchainImages;
	auto swapchainImageSize = static_cast<uint32_t>(swapchainImages.size());

	auto& descriptorPool = globalInfoComp->descriptorPool;

	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.maxSets = swapchainImageSize;

	//定义描述符池可以分配的描述符集
	std::array<VkDescriptorPoolSize, 2> sizes = {};

	sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	sizes[0].descriptorCount = swapchainImageSize;

	sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	sizes[1].descriptorCount = swapchainImageSize;

	createInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
	createInfo.pPoolSizes = sizes.data();

	auto ret = vkCreateDescriptorPool(logicDevice, &createInfo, nullptr, &descriptorPool);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create descriptorPool error!");
	}
}

void RenderSystem::DestroyDescriptorPool(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& descriptorPool = globalInfoComp->descriptorPool;
	vkDestroyDescriptorPool(logicDevice, descriptorPool, nullptr);
}

//通过描述符池创建描述符集对象了
void RenderSystem::AllocateDescriptorSets(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	auto& swapchainImages = globalInfoComp->swapchainImages;
	auto swapchainImageSize = static_cast<uint32_t>(swapchainImages.size());

	auto& descriptorSetLayout = globalInfoComp->descriptorSetLayout;
	auto& descriptorPool = globalInfoComp->descriptorPool;
	auto& descriptorSets = globalInfoComp->descriptorSets;

	descriptorSets.resize(swapchainImageSize);

	VkDescriptorSetAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.descriptorPool = descriptorPool;
	allocateInfo.descriptorSetCount = swapchainImageSize;

	//使用的描述符布局
	//	描述符布局对象个数要匹配描述符集对象个数
	std::vector<VkDescriptorSetLayout> layouts(swapchainImageSize, descriptorSetLayout);
	allocateInfo.pSetLayouts = layouts.data();

	auto ret = vkAllocateDescriptorSets(logicDevice, &allocateInfo, descriptorSets.data());
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create allocateDescriptorSet error!");
	}

	auto& uniformBuffers = globalInfoComp->uniformBuffers;
	auto& textureSampler = globalInfoComp->textureSampler;
	auto& textureImageView = globalInfoComp->textureImageView;

	//配置描述符集对象创建
	for (auto i = 0; i < swapchainImageSize; i++) {

		//vkUpdateDescriptorSets 更新描述符的配置
		std::array<VkWriteDescriptorSet, 2> writes = {};

		writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[0].dstSet = descriptorSets[i]; //指定要更新的描述符集对象
		writes[0].dstBinding = 0;
		writes[0].dstArrayElement = 0; //index

		//指定描述符的类型
		writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writes[0].descriptorCount = 1;

		//指定缓冲数据
		//	VkDescriptorBufferInfo 配置描述符引用的缓冲对象
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject); //可以访问的数据范围
		writes[0].pBufferInfo = &bufferInfo;

		writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[1].dstSet = descriptorSets[i];
		writes[1].dstBinding = 1;
		writes[1].dstArrayElement = 0;
		writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writes[1].descriptorCount = 1;

		//指定图像数据
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.sampler = textureSampler;
		imageInfo.imageView = textureImageView;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		writes[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(logicDevice, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
	}
}