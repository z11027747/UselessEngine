#pragma once

#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/image/image_comp.h"
#include "render/image/image2d_system.h"
#include "render/cmd/cmd_submit_system.h"
#include "context.h"

namespace Render {

	//创建设备内存(显存)支持的图像对象
	std::shared_ptr<Image2D> Image2DSystem::Create(Context* context,
		VkFormat format, VkExtent2D extent,
		VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags propertiesFlags
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		//创建vkImage对象
		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = format;
		createInfo.extent = { extent.width, extent.height, 1 }; //2d depth=1
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

		VkImage vkImage;
		auto ret = vkCreateImage(logicalDevice, &createInfo, nullptr, &vkImage);
		CheckRet(ret, "vkCreateImage");

		//申请内存绑定数据
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(logicalDevice, vkImage, &requirements);

		auto memoryTypeIndex = PhysicalDeviceSystem::FindMemoryType(context,
			requirements.memoryTypeBits, propertiesFlags);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = requirements.size;
		allocateInfo.memoryTypeIndex = memoryTypeIndex;

		VkDeviceMemory vkDeviceMemory;
		auto allocateRet = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &vkDeviceMemory);
		CheckRet(allocateRet, "vkAllocateMemory");

		auto bindRet = vkBindImageMemory(logicalDevice, vkImage, vkDeviceMemory, 0);
		CheckRet(bindRet, "vkBindImageMemory");

		auto image2d = std::shared_ptr<Image2D>();
		image2d->fomat = format;
		image2d->extent = extent;
		image2d->vkImage = vkImage;
		image2d->vkDeviceMemory = vkDeviceMemory;
		return image2d;
	}

	void Image2DSystem::Destroy(Context* context,
		std::shared_ptr<Image2D> image2d
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyImage(logicalDevice, image2d->vkImage, nullptr);
		vkFreeMemory(logicalDevice, image2d->vkDeviceMemory, nullptr);

		if (image2d->vkImageView != nullptr) {
			vkDestroyImageView(logicalDevice, image2d->vkImageView, nullptr);
		}
	}

	//任何VkImage对象，都需要创建一个VkImageView对象来绑定访问它
	void Image2DSystem::CreateImageView(Context* context,
		std::shared_ptr<Image2D> image2d,
		VkImageAspectFlags aspectMask
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto format = image2d->fomat;
		auto& vkImage = image2d->vkImage;

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = vkImage;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;

		//进行图像颜色通道的映射
		auto identity = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components = { identity,identity,identity,identity };

		//指定图像的用途和图像的哪一部分可以被访问
		createInfo.subresourceRange = { aspectMask,0,1,0,1 };

		VkImageView vkImageView;
		auto ret = vkCreateImageView(logicalDevice, &createInfo, nullptr, &vkImageView);
		CheckRet(ret, "vkCreateImageView");

		image2d->vkImageView = vkImageView;
	}

	void Image2DSystem::TransitionLayout(Context* context,
		std::shared_ptr<Image2D> image2d,
		VkImageLayout oldLayout, VkImageLayout newLayout,
		VkImageAspectFlags aspectMask,
		VkAccessFlags srcAccessMas, VkAccessFlags dstAccessMas,
		VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage
	) {

		CmdSubmitSystem::Create(context,
			[=](VkCommandBuffer& cmdBuffer)
			{
				VkImageMemoryBarrier imageMemoryBarrier = {};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.oldLayout = oldLayout;
				imageMemoryBarrier.newLayout = newLayout;
				imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageMemoryBarrier.image = image2d->vkImage;
				imageMemoryBarrier.subresourceRange = { aspectMask,0,1,0,1 };
				imageMemoryBarrier.srcAccessMask = srcAccessMas;
				imageMemoryBarrier.dstAccessMask = dstAccessMas;

				vkCmdPipelineBarrier(cmdBuffer,
					srcStage, dstStage,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			});

	}

}