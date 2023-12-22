
#include "render/comp.h"
#include "tool/system.h"
#include "context.h"

//使用纹理，需要采取下面的步骤：
//	1.创建设备内存(显存)支持的图像对象
//	2.加载图像文件的像素数据
//	3.创建图像采样器
//	4.使用图像采样器描述符采样纹理数据

//创建Image
void RenderSystem::CreateImage(Context* context,
	//image
	uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
	//memory,
	VkMemoryPropertyFlags propertyFlags,
	//return
	VkImage& image, VkDeviceMemory& imageMemory)
{
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	//创建vkImage对象
	VkImageCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.format = format;
	createInfo.extent.width = width;
	createInfo.extent.height = height;
	createInfo.extent.depth = 1; //二维图像，depth设置为1
	createInfo.mipLevels = 1;
	createInfo.arrayLayers = 1;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	//VkImageTiling 图像数据格式
	//	VK_IMAGE_TILING_LINEAR：纹素以行主序的方式排列
	//	VK_IMAGE_TILING_OPTIMAL：纹素以一种对访问优化的方式排列（不需要直接访问就用这个）
	createInfo.tiling = tiling;
	createInfo.usage = usage; //被着色器采样
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//VkImageLayout	图像数据在内存中的排列方式和使用情况
	//	VK_IMAGE_LAYOUT_UNDEFINED： 图像的初始布局
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	auto ret = vkCreateImage(logicDevice, &createInfo, nullptr, &image);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create textureImage error!");
	}

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(logicDevice, image, &requirements);

	auto memoryTypeIndex = FindMemoryType(context,
		requirements.memoryTypeBits, propertyFlags);

	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = requirements.size;
	allocateInfo.memoryTypeIndex = memoryTypeIndex;

	auto allocateRet = vkAllocateMemory(logicDevice, &allocateInfo, nullptr, &imageMemory);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("allocate textureImageMemory error!");
	}

	vkBindImageMemory(logicDevice, image, imageMemory, 0);
}

//销毁Image
void RenderSystem::DestroyImage(Context* context, VkImage& image, VkDeviceMemory& imageMemory) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	vkDestroyImage(logicDevice, image, nullptr);
	vkFreeMemory(logicDevice, imageMemory, nullptr);
}

//布局变化
void RenderSystem::TransitionImageLayout(Context* context,
	VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout) {

	//内存屏障(image memory barrier)我们可以对图像布局进行变换。
	//管线屏障(pipeline barrier)主要被用来同步资源访问，比如保证图像在被读取之前数据被写入。

	RecordCommandBufferSingleTime(context,
		[oldLayout, newLayout, &image](VkCommandBuffer& commandBuffer)
		{
			VkImageMemoryBarrier memoryBarrier = {};
			memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

			//如果不需要访问之前的图像数据
			//	可以将oldLayout设置为VK_IMAGE_LAYOUT_UNDEFINED来获得更好的性能表现
			memoryBarrier.oldLayout = oldLayout;
			memoryBarrier.newLayout = newLayout;

			//是否需要传递队列族的所有权
			memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			memoryBarrier.image = image;

			//subresourceRange 影响的图像范围
			if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
				memoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			}
			else {
				memoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; //只涉及图像的颜色方面（深度、模板等）
			}
			memoryBarrier.subresourceRange.baseMipLevel = 0;
			memoryBarrier.subresourceRange.levelCount = 1;
			memoryBarrier.subresourceRange.baseArrayLayer = 0;
			memoryBarrier.subresourceRange.layerCount = 1;

			//vkCmdPipelineBarrier 提交管线屏障对象
			//	srcStageMask 指定发生在屏障之前的管线阶段
			//	dstStageMask 指定发生在屏障之后的管线阶段
			VkPipelineStageFlags sourceStage;
			VkPipelineStageFlags destinationStage;

			//未定义->传输目的：传输操作的数据写入不需要等待
			if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; //最早出现的管线阶段
				destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT; //传输操作阶段，例如缓冲区和图像的复制

				//指定在屏障之前必须发生的资源操作类型，以及必须等待屏障的资源操作类型。
				memoryBarrier.srcAccessMask = VK_ACCESS_NONE;
				memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			}
			//传输目的->着色器读取：着色器读取图像数据需要等待传输操作的写入结束
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
				sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT; //最早出现的管线阶段
				destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; //传输操作阶段，例如缓冲区和图像的复制

				memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			}
			//未定义->深度测试: 不用等
			else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
				sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; //用来检测片段是否可以覆盖之前的片段

				memoryBarrier.srcAccessMask = 0;
				memoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			}
			else {
				throw std::invalid_argument("unsupported layout transition!");
			}

			//	后六个参数：
			vkCmdPipelineBarrier(commandBuffer,
				sourceStage, destinationStage,
				0,
				0, nullptr, //内存屏障(memory barriers)
				0, nullptr, //缓冲内存屏障(buffer memory barriers)
				1, &memoryBarrier //图像内存屏障(image memory barriers)。
			);

		});
}

//拷贝Buffer内的数据到Image
void RenderSystem::CopyBufferToImage(Context* context,
	VkBuffer& srcBuffer, VkImage& dstImage, uint32_t width, uint32_t height) {

	RecordCommandBufferSingleTime(context,
		[&srcBuffer, &dstImage, width, height](VkCommandBuffer& commandBuffer)
		{
			VkBufferImageCopy imageCopy = {};

			imageCopy.bufferOffset = 0; //要复制的数据在缓冲中的偏移位置
			imageCopy.bufferRowLength = 0; //于指定数据在内存中的存放方式，可以加额外对齐
			imageCopy.bufferImageHeight = 0;

			imageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopy.imageSubresource.mipLevel = 0;
			imageCopy.imageSubresource.baseArrayLayer = 0;
			imageCopy.imageSubresource.layerCount = 1;

			imageCopy.imageOffset = { 0, 0, 0 };
			imageCopy.imageExtent = { width, height, 1 };

			//可以指定数组来一次从一个缓冲复制数据到多个不同的图像对象
			vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);
		});

}

void RenderSystem::CreateImageView(Context* context,
	VkFormat format, VkImageAspectFlags aspectMask,
	VkImage& image, VkImageView& imageView) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;

	//进行图像颜色通道的映射
	createInfo.components = {
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY };

	//指定图像的用途和图像的哪一部分可以被访问
	createInfo.subresourceRange = { aspectMask,0,1,0,1 };

	auto ret = vkCreateImageView(logicDevice, &createInfo, nullptr, &imageView);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create imageView error");
	}
}

void RenderSystem::DestroyImageView(Context* context, VkImageView& imageView) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	vkDestroyImageView(logicDevice, imageView, nullptr);
}

void RenderSystem::CreateTextureImage(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	auto& textureImage = globalInfoComp->textureImage;
	auto& textureImageMemory = globalInfoComp->textureImageMemory;

	int imgWidth, imgHeight;
	auto imageData = ToolSystem::LoadImg("resource/texture/icon2.png", imgWidth, imgHeight);

	//分配图像内存
	VkDeviceSize imageSize = imgWidth * imgHeight * 4;

	//创建一个CPU可见的缓冲，图像像素数据复制到其中
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBuffer(context,
		imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(logicDevice, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, imageData, static_cast<size_t>(imageSize));
	vkUnmapMemory(logicDevice, stagingBufferMemory);

	ToolSystem::FreeImg(imageData);

	//可以在着色器直接访问缓冲中的像素数据，但使用Vulkan的图像对象会更好
	//Vulkan的图像对象允许使用二维坐标来快速获取颜色数据
	//图像对象的像素数据也被叫做纹素

	auto width = static_cast<uint32_t>(imgWidth);
	auto height = static_cast<uint32_t>(imgHeight);

	CreateImage(context,
		//image
		width, height, VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL, //纹素以一种对访问优化的方式排列
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, //给采样器用
		//memory
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		//return
		textureImage, textureImageMemory);

	//CopyImage 之前需要先做布局变化

	//转到dst传输目标格式
	TransitionImageLayout(context,
		textureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(context,
		stagingBuffer, textureImage, imgWidth, imgHeight);

	DestroyBuffer(context, stagingBuffer, stagingBufferMemory);

	//转换到着色器可以采样的格式
	TransitionImageLayout(context,
		textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	//TODO
	//优化：推荐组合多个操作到一个指令缓冲对象，通过异步执行来增大吞吐量
	//	特别对于createTextureImage函数中的变换和数据复制操作
}

void RenderSystem::DestroyTextureImage(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	auto& textureImage = globalInfoComp->textureImage;
	auto& textureImageMemory = globalInfoComp->textureImageMemory;
	DestroyImage(context, textureImage, textureImageMemory);
}

void RenderSystem::CreateTextureImageView(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();

	auto& textureImage = globalInfoComp->textureImage;
	auto& textureImageView = globalInfoComp->textureImageView;
	CreateImageView(context,
		VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT,
		textureImage, textureImageView);
}

void RenderSystem::DestroyTextureImageView(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();

	auto& textureImageView = globalInfoComp->textureImageView;
	DestroyImageView(context, textureImageView);
}

void RenderSystem::CreateTextureSampler(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;

	VkSamplerCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	//纹理放大和缩小时的采样方式
	createInfo.magFilter = VK_FILTER_LINEAR;
	createInfo.minFilter = VK_FILTER_LINEAR;

	//UVW 对应xyz三个轴的寻址模式
	createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	//各向异性
	createInfo.anisotropyEnable = false;
	createInfo.maxAnisotropy = 1;

	//VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER 寻址模式时采样超出图像范围时返回的边界颜色
	createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	//false 时采样坐标是 0-1
	createInfo.unnormalizedCoordinates = false;

	//compare 阴影相关
	//createInfo.compareEnable = false;
	//createInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	//mipmap
	createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	createInfo.mipLodBias = 0.0f;
	createInfo.minLod = 0.0f;
	createInfo.maxLod = 0.0f;

	auto ret = vkCreateSampler(logicDevice, &createInfo, nullptr, &globalInfoComp->textureSampler);
	if (ret != VK_SUCCESS) {
		throw std::runtime_error("create sampler error");
	}
}

void RenderSystem::DestroyTextureSampler(Context* context) {
	auto& renderEO = context->renderEO;

	auto globalInfoComp = renderEO->GetComponent<RenderGlobalComp>();
	auto& logicDevice = globalInfoComp->logicDevice;
	auto& textureSampler = globalInfoComp->textureSampler;

	vkDestroySampler(logicDevice, textureSampler, nullptr);
}