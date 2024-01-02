
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "context.h"

namespace Render {

	std::shared_ptr<Image> ImageLogic::CreateByInfo(Context* context,
		ImageInfo& info) {

		auto image = std::make_shared<Image>();

		image->fomat = info.format;
		image->extent = info.extent;
		image->aspectMask = info.aspectMask;
		image->layerCount = info.arrayLayers;

		Create(context,
			image, info);

		CreateView(context,
			image,
			info.viewType, info.aspectMask, info.arrayLayers);

		TransitionLayout(context,
			image,
			info.oldLayout, info.newLayout,
			info.aspectMask,
			info.srcAccessMask, info.dstAccessMask,
			info.srcStage, info.dstStage);

		return image;
	}

	void ImageLogic::Create(Context* context,
		std::shared_ptr<Image> image, ImageInfo& info) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.flags = info.flags;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = info.format;
		createInfo.extent = { info.extent.width, info.extent.height, 1 }; //2d depth=1
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = info.arrayLayers;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = info.tiling;
		createInfo.usage = info.usage;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImage vkImage;
		auto ret = vkCreateImage(logicalDevice, &createInfo, nullptr, &vkImage);
		CheckRet(ret, "vkCreateImage");

		//申请内存绑定数据
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(logicalDevice, vkImage, &requirements);

		auto memoryTypeIndex = PhysicalDeviceLogic::FindMemoryType(context,
			requirements.memoryTypeBits, info.propertiesFlags);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = requirements.size;
		allocateInfo.memoryTypeIndex = memoryTypeIndex;

		VkDeviceMemory vkDeviceMemory;
		auto allocateRet = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &vkDeviceMemory);
		CheckRet(allocateRet, "vkAllocateMemory");

		auto bindRet = vkBindImageMemory(logicalDevice, vkImage, vkDeviceMemory, 0);
		CheckRet(bindRet, "vkBindImageMemory");

		image->vkImage = vkImage;
		image->vkDeviceMemory = vkDeviceMemory;
		image->size = requirements.size;
	}

	void ImageLogic::Destroy(Context* context,
		std::shared_ptr<Image> image
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyImage(logicalDevice, image->vkImage, nullptr);
		vkFreeMemory(logicalDevice, image->vkDeviceMemory, nullptr);

		if (image->vkImageView != nullptr) {
			DestroyView(context, image);
		}
	}

	void ImageLogic::CreateView(Context* context,
		std::shared_ptr<Image> image,
		VkImageViewType viewType, VkImageAspectFlags aspectMask, uint32_t layerCount
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto format = image->fomat;
		auto& vkImage = image->vkImage;

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = vkImage;
		createInfo.viewType = viewType;
		createInfo.format = format;
		createInfo.components = {
			VK_COMPONENT_SWIZZLE_IDENTITY,
			 VK_COMPONENT_SWIZZLE_IDENTITY,
			 VK_COMPONENT_SWIZZLE_IDENTITY,
			 VK_COMPONENT_SWIZZLE_IDENTITY
		};
		createInfo.subresourceRange = { aspectMask,0,1,0,layerCount };

		VkImageView vkImageView;
		auto ret = vkCreateImageView(logicalDevice, &createInfo, nullptr, &vkImageView);
		CheckRet(ret, "vkCreateImageView");

		image->vkImageView = vkImageView;
	}

	void ImageLogic::DestroyView(Context* context,
		std::shared_ptr<Image> image
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		vkDestroyImageView(logicalDevice, image->vkImageView, nullptr);
	}

	void ImageLogic::TransitionLayout(Context* context,
		std::shared_ptr<Image> image,
		VkImageLayout oldLayout, VkImageLayout newLayout,
		VkImageAspectFlags aspectMask,
		VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
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
				imageMemoryBarrier.image = image->vkImage;
				imageMemoryBarrier.subresourceRange = { image->aspectMask,0,1,0,image->layerCount };
				imageMemoryBarrier.srcAccessMask = srcAccessMask;
				imageMemoryBarrier.dstAccessMask = dstAccessMask;

				vkCmdPipelineBarrier(cmdBuffer,
					srcStage, dstStage,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			});

		image->layout = newLayout;
	}

	void ImageLogic::CopyBuffer(Context* context,
		std::shared_ptr<Image> image,
		std::shared_ptr<Buffer> buffer
	) {
		CmdSubmitLogic::Create(context,
			[&](VkCommandBuffer& cmdBuffer)
			{
				VkBufferImageCopy imageCopy = {};
				imageCopy.bufferOffset = 0;
				imageCopy.bufferRowLength = 0;
				imageCopy.bufferImageHeight = 0;
				imageCopy.imageSubresource.aspectMask = image->aspectMask;
				imageCopy.imageSubresource.mipLevel = 0;
				imageCopy.imageSubresource.baseArrayLayer = 0;
				imageCopy.imageSubresource.layerCount = image->layerCount;
				imageCopy.imageOffset = { 0, 0, 0 };
				imageCopy.imageExtent = { image->extent.width, image->extent.height, 1 };

				vkCmdCopyBufferToImage(cmdBuffer, buffer->vkBuffer, image->vkImage,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);
			});
	}

}