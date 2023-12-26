#pragma once

#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image2d_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "context.h"

namespace Render {

	std::shared_ptr<Image2D> Image2DLogic::Create(Context* context,
		VkFormat format, VkExtent2D extent,
		VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags propertiesFlags
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = format;
		createInfo.extent = { extent.width, extent.height, 1 }; //2d depth=1
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = tiling;
		createInfo.usage = usage;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImage vkImage;
		auto ret = vkCreateImage(logicalDevice, &createInfo, nullptr, &vkImage);
		CheckRet(ret, "vkCreateImage");

		//申请内存绑定数据
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(logicalDevice, vkImage, &requirements);

		auto memoryTypeIndex = PhysicalDeviceLogic::FindMemoryType(context,
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

		auto image2d = std::make_shared<Image2D>();
		image2d->fomat = format;
		image2d->extent = extent;
		image2d->vkImage = vkImage;
		image2d->vkDeviceMemory = vkDeviceMemory;
		return image2d;
	}

	void Image2DLogic::Destroy(Context* context,
		std::shared_ptr<Image2D> image2d
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyImage(logicalDevice, image2d->vkImage, nullptr);
		vkFreeMemory(logicalDevice, image2d->vkDeviceMemory, nullptr);

		if (image2d->vkImageView != nullptr) {
			DestroyView(context, image2d);
		}
	}

	void Image2DLogic::CreateView(Context* context,
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
		createInfo.components = {
			VK_COMPONENT_SWIZZLE_IDENTITY,
			 VK_COMPONENT_SWIZZLE_IDENTITY,
			 VK_COMPONENT_SWIZZLE_IDENTITY,
			 VK_COMPONENT_SWIZZLE_IDENTITY
		};
		createInfo.subresourceRange = { aspectMask,0,1,0,1 };

		VkImageView vkImageView;
		auto ret = vkCreateImageView(logicalDevice, &createInfo, nullptr, &vkImageView);
		CheckRet(ret, "vkCreateImageView");

		image2d->vkImageView = vkImageView;
	}

	void Image2DLogic::DestroyView(Context* context,
		std::shared_ptr<Image2D> image2d
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyImageView(logicalDevice, image2d->vkImageView, nullptr);
	}

	void Image2DLogic::TransitionLayout(Context* context,
		std::shared_ptr<Image2D> image2d,
		VkImageLayout oldLayout, VkImageLayout newLayout,
		VkImageAspectFlags aspectMask,
		VkAccessFlags srcAccessMas, VkAccessFlags dstAccessMas,
		VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage
	) {

		CmdSubmitLogic::Create(context,
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