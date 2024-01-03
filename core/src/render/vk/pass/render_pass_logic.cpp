
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/image/image_logic.h"
#include "context.h"

namespace Render
{
	void RenderPassLogic::CreateColorAttachment(Context *context,
												std::shared_ptr<Pass> pass,
												VkAttachmentLoadOp loadOp,
												VkImageLayout initLayout, VkImageLayout finalLayout)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto surfaceFormat = global->surfaceFormat;

		VkAttachmentDescription colorAttachmentDescription = {};
		colorAttachmentDescription.format = surfaceFormat.format;
		colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentDescription.loadOp = loadOp;
		colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentDescription.initialLayout = initLayout;
		colorAttachmentDescription.finalLayout = finalLayout;

		VkAttachmentReference colorAttachmentReference = {};
		colorAttachmentReference.attachment = 0;
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		pass->attachmentDescriptions.push_back(colorAttachmentDescription);
		pass->colorAttachmentReference = colorAttachmentReference;
		pass->clearColorValue = {0.0f, 0.0f, 0.0f, 0.0f};
	}

	void RenderPassLogic::CreateDepthAttachment(Context *context,
												std::shared_ptr<Pass> pass, uint32_t index)
	{
		VkAttachmentDescription depthAttachmentDescription = {};
		depthAttachmentDescription.format = VK_FORMAT_D16_UNORM;
		depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

		VkAttachmentReference depthAttachmentReference = {};
		depthAttachmentReference.attachment = index;
		depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		pass->attachmentDescriptions.push_back(depthAttachmentDescription);
		pass->depthAttachmentReference = depthAttachmentReference;
		pass->clearDepthValue = {1.0f, 0};
	}

	void RenderPassLogic::GetSwapchainImage2ds(Context *context,
											   std::shared_ptr<Pass> pass)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;
		auto swapchainImageCount = global->swapchainImageCount;
		auto &swapchainImages = global->swapchainImages;

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			pass->colorImage2ds.push_back(swapchainImages[i]);
		}
		pass->isGetSwapchainImage = true;
	}

	void RenderPassLogic::CreateColorImage2ds(Context *context,
											  std::shared_ptr<Pass> pass)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto swapchainImageCount = global->swapchainImageCount;

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			ImageInfo image2dInfo = {
				VK_FORMAT_R8G8B8A8_UNORM, {currentExtent.width, currentExtent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
				// image
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				0,
				1,
				VK_IMAGE_VIEW_TYPE_2D,
				// memory
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				// layout
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
			auto colorImage2d = ImageLogic::CreateByInfo(context,
														 image2dInfo);

			pass->colorImage2ds.push_back(colorImage2d);
		}
	}

	void RenderPassLogic::CreateDepthImage2ds(Context *context,
											  std::shared_ptr<Pass> pass)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto swapchainImageCount = global->swapchainImageCount;

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			ImageInfo image2dInfo = {
				VK_FORMAT_D16_UNORM, {currentExtent.width, currentExtent.height, 0}, VK_IMAGE_ASPECT_DEPTH_BIT,
				// image
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				0,
				1,
				VK_IMAGE_VIEW_TYPE_2D,
				// memory
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				// layout
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

			auto depthImage2d = ImageLogic::CreateByInfo(context,
														 image2dInfo);

			pass->depthImage2ds.push_back(depthImage2d);
		}
	}

	void RenderPassLogic::DestroyColorImage2ds(Context *context,
											   std::shared_ptr<Pass> pass)
	{
		if (pass->isGetSwapchainImage)
		{
			pass->colorImage2ds.clear();
			return;
		}

		auto &colorImage2ds = pass->colorImage2ds;
		for (const auto &colorImage2d : colorImage2ds)
		{
			ImageLogic::Destroy(context, colorImage2d);
		}
		colorImage2ds.clear();
	}

	void RenderPassLogic::DestroyDepthImage2ds(Context *context,
											   std::shared_ptr<Pass> pass)
	{
		auto &depthImage2ds = pass->depthImage2ds;
		for (const auto &depthImage2d : depthImage2ds)
		{
			ImageLogic::Destroy(context, depthImage2d);
		}
		depthImage2ds.clear();
	}

	void RenderPassLogic::AddSubpassDependency(Context *context,
											   std::shared_ptr<Pass> pass,
											   uint32_t srcSubpass, uint32_t dstSubpass,
											   VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
											   VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask)
	{
		VkSubpassDependency subpassDependency = {};
		subpassDependency.srcSubpass = srcSubpass;
		subpassDependency.dstSubpass = dstSubpass;
		subpassDependency.srcStageMask = srcStageMask;
		subpassDependency.dstStageMask = dstStageMask;
		subpassDependency.srcAccessMask = srcAccessMask;
		subpassDependency.dstAccessMask = dstAccessMask;
		// subpassDependency.dependencyFlags = dependencyFlags;

		pass->subpassDependencies.push_back(subpassDependency);
	}

	void RenderPassLogic::SetSubPassDescription(Context *context,
												std::shared_ptr<Pass> pass)
	{
		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		if (pass->colorImage2ds.size() > 0)
		{
			subpassDescription.colorAttachmentCount = 1;
			subpassDescription.pColorAttachments = &pass->colorAttachmentReference;
		}

		if (pass->depthImage2ds.size() > 0)
		{
			subpassDescription.pDepthStencilAttachment = &pass->depthAttachmentReference;
		}

		pass->subpassDescription = subpassDescription;
	}

	void RenderPassLogic::Create(Context *context,
								 std::shared_ptr<Pass> pass)
	{
		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = static_cast<uint32_t>(pass->attachmentDescriptions.size());
		createInfo.pAttachments = pass->attachmentDescriptions.data();
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &pass->subpassDescription;
		createInfo.dependencyCount = static_cast<uint32_t>(pass->subpassDependencies.size());
		createInfo.pDependencies = pass->subpassDependencies.data();

		VkRenderPass vkRenderPass;
		auto ret = vkCreateRenderPass(LogicalDeviceLogic::Get(context),
									  &createInfo, nullptr, &vkRenderPass);
		CheckRet(ret, "vkCreateRenderPass");

		pass->renderPass = vkRenderPass;
	}

	void RenderPassLogic::Destroy(Context *context,
								  std::shared_ptr<Pass> pass)
	{
		auto &renderPass = pass->renderPass;
		vkDestroyRenderPass(LogicalDeviceLogic::Get(context),
							renderPass, nullptr);
	}

}
