
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	std::shared_ptr<Image> ImageLogic::CreateByInfo(Context *context,
													ImageCreateInfo &createInfo)
	{
		auto image = std::make_shared<Image>();

		image->format = createInfo.format;
		image->extent = createInfo.extent;
		image->aspectMask = createInfo.aspectMask;
		image->layerCount = createInfo.layerCount;
		image->mipLevels = createInfo.mipLevels;

		Create(context,
			   image, createInfo);

		CreateView(context,
				   image,
				   createInfo.viewType, createInfo.aspectMask, createInfo.layerCount, createInfo.mipLevels);

		TransitionLayout(context,
						 image,
						 createInfo.oldLayout, createInfo.newLayout, createInfo.mipLevels);

		return image;
	}

	void ImageLogic::Create(Context *context,
							std::shared_ptr<Image> image, ImageCreateInfo &createInfo)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;

		VkImageCreateInfo vkCreateInfo = {};
		vkCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vkCreateInfo.flags = createInfo.flags;
		vkCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		vkCreateInfo.format = createInfo.format;
		vkCreateInfo.extent = {createInfo.extent.width, createInfo.extent.height, 1}; // 2d depth=1
		vkCreateInfo.mipLevels = createInfo.mipLevels;
		vkCreateInfo.arrayLayers = createInfo.layerCount;
		vkCreateInfo.samples = createInfo.samplers;
		vkCreateInfo.tiling = createInfo.tiling;
		vkCreateInfo.usage = createInfo.usage;
		vkCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImage vkImage;
		auto ret = vkCreateImage(logicalDevice, &vkCreateInfo, nullptr, &vkImage);
		CheckRet(ret, "vkCreateImage");

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(logicalDevice, vkImage, &requirements);

		auto memoryTypeIndex = PhysicalDeviceLogic::FindMemoryType(context,
																   requirements.memoryTypeBits, createInfo.propertiesFlags);

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

	void ImageLogic::Destroy(Context *context,
							 std::shared_ptr<Image> image)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;

		vkDestroyImage(logicalDevice, image->vkImage, nullptr);
		vkFreeMemory(logicalDevice, image->vkDeviceMemory, nullptr);

		if (image->vkImageView != nullptr)
		{
			DestroyView(context, image);
		}
	}

	void ImageLogic::CreateView(Context *context,
								std::shared_ptr<Image> image,
								VkImageViewType viewType, VkImageAspectFlags aspectMask, uint32_t layerCount, uint32_t mipLevels)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;

		auto format = image->format;
		auto &vkImage = image->vkImage;

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = vkImage;
		createInfo.viewType = viewType;
		createInfo.format = format;
		createInfo.components = {
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY};
		createInfo.subresourceRange = {aspectMask, 0, mipLevels, 0, layerCount};

		VkImageView vkImageView;
		auto ret = vkCreateImageView(logicalDevice, &createInfo, nullptr, &vkImageView);
		CheckRet(ret, "vkCreateImageView");

		image->vkImageView = vkImageView;
	}

	void ImageLogic::DestroyView(Context *context,
								 std::shared_ptr<Image> image)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;

		vkDestroyImageView(logicalDevice, image->vkImageView, nullptr);
	}

	void ImageLogic::TransitionLayout(Context *context,
									  std::shared_ptr<Image> image,
									  VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels,
									  bool singleTime)
	{
		auto cmdBuffer = CmdSubmitLogic::CreateAndBegin(context);

		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.oldLayout = oldLayout;
		imageMemoryBarrier.newLayout = newLayout;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.image = image->vkImage;
		imageMemoryBarrier.subresourceRange = {image->aspectMask, 0, mipLevels, 0, image->layerCount};

		VkPipelineStageFlags srcStage;
		VkPipelineStageFlags dstStage;
		if ((oldLayout == VK_IMAGE_LAYOUT_UNDEFINED || oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) &&
			(newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL || newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL))
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_NONE;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if ((oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL || oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) &&
				 newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_NONE;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_NONE;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
				 (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL ||
				  newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
				  newLayout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL))
		{
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_NONE;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			Common::Log::Error("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(cmdBuffer,
							 srcStage, dstStage,
							 0,
							 0, nullptr,
							 0, nullptr,
							 1, &imageMemoryBarrier);

		if (singleTime)
			CmdSubmitLogic::EndSingleTime(context, cmdBuffer);
		else
			CmdSubmitLogic::End(context, cmdBuffer);

		image->layout = newLayout;
	}

	void ImageLogic::CopyFromBuffer(Context *context,
									std::shared_ptr<Image> image,
									std::shared_ptr<Buffer> buffer)
	{
		auto cmdBuffer = CmdSubmitLogic::CreateAndBegin(context);

		VkBufferImageCopy imageCopy = {};
		imageCopy.bufferOffset = 0;
		imageCopy.bufferRowLength = 0;
		imageCopy.bufferImageHeight = 0;
		imageCopy.imageSubresource.aspectMask = image->aspectMask;
		imageCopy.imageSubresource.mipLevel = 0;
		imageCopy.imageSubresource.baseArrayLayer = 0;
		imageCopy.imageSubresource.layerCount = image->layerCount;
		imageCopy.imageOffset = {0, 0, 0};
		imageCopy.imageExtent = {image->extent.width, image->extent.height, 1};

		vkCmdCopyBufferToImage(cmdBuffer, buffer->vkBuffer, image->vkImage,
							   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopy);

		CmdSubmitLogic::End(context, cmdBuffer);
	}

	void ImageLogic::CopyFromImage(Context *context,
								   std::shared_ptr<Image> image,
								   std::shared_ptr<Image> srcImage, bool singleTime)
	{
		auto cmdBuffer = CmdSubmitLogic::CreateAndBegin(context);

		VkImageCopy copyRegion = {};
		copyRegion.srcSubresource.aspectMask = srcImage->aspectMask;
		copyRegion.srcSubresource.layerCount = srcImage->layerCount;
		copyRegion.dstSubresource.aspectMask = image->aspectMask;
		copyRegion.dstSubresource.layerCount = image->layerCount;
		copyRegion.extent = {image->extent.width, image->extent.height, 1};

		vkCmdCopyImage(cmdBuffer,
					   srcImage->vkImage, srcImage->layout,
					   image->vkImage, image->layout,
					   1, &copyRegion);

		if (singleTime)
			CmdSubmitLogic::EndSingleTime(context, cmdBuffer);
		else
			CmdSubmitLogic::End(context, cmdBuffer);
	}

	void ImageLogic::GenerateMipmapsAndTransitionLayout(Context *context,
														std::shared_ptr<Image> image,
														VkImageLayout oldLayout, VkImageLayout newLayout,
														bool singleTime)
	{
		auto cmdBuffer = CmdSubmitLogic::CreateAndBegin(context);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image->vkImage;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = image->aspectMask;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = image->layerCount;
		barrier.subresourceRange.levelCount = 1;

		auto mipWidth = static_cast<int32_t>(image->extent.width);
		auto mipHeight = static_cast<int32_t>(image->extent.height);
		auto mipLevels = image->mipLevels;

		for (auto i = 1u; i < mipLevels; i++)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(cmdBuffer,
								 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
								 0,
								 0, nullptr,
								 0, nullptr,
								 1, &barrier);

			VkImageBlit blit = {};
			blit.srcOffsets[0] = {0, 0, 0};
			blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
			blit.srcSubresource.aspectMask = image->aspectMask;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = image->layerCount;
			blit.dstOffsets[0] = {0, 0, 0};
			blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
			blit.dstSubresource.aspectMask = image->aspectMask;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = image->layerCount;

			vkCmdBlitImage(cmdBuffer,
						   image->vkImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						   image->vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						   1, &blit, VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = newLayout;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(cmdBuffer,
								 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
								 0,
								 0, nullptr,
								 0, nullptr,
								 1, &barrier);

			if (mipWidth > 1)
				mipWidth /= 2;
			if (mipHeight > 1)
				mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(cmdBuffer,
							 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
							 0,
							 0, nullptr,
							 0, nullptr,
							 1, &barrier);

		if (singleTime)
			CmdSubmitLogic::EndSingleTime(context, cmdBuffer);
		else
			CmdSubmitLogic::End(context, cmdBuffer);

		image->layout = newLayout;
	}
}