
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void PassLogic::CreateColorAttachment(Context *context,
										  std::shared_ptr<Pass> pass,
										  VkSampleCountFlagBits samplers, VkImageLayout initLayout, VkImageLayout finalLayout)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto surfaceFormat = global->surfaceFormat;

		VkAttachmentDescription colorAttachmentDescription = {};
		colorAttachmentDescription.format = surfaceFormat.format;
		colorAttachmentDescription.samples = samplers;
		colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
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
		pass->clearColorValue = {0.0, 0.0, 0.0, 0.0f};
	}

	void PassLogic::CreateDepthAttachment(Context *context,
										  std::shared_ptr<Pass> pass,
										  VkSampleCountFlagBits samplers, uint32_t index)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto depthFormat = global->depthFormat;

		VkAttachmentDescription depthAttachmentDescription = {};
		depthAttachmentDescription.format = depthFormat;
		depthAttachmentDescription.samples = samplers;
		depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

		VkAttachmentReference depthAttachmentReference = {};
		depthAttachmentReference.attachment = index;
		depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		pass->attachmentDescriptions.push_back(depthAttachmentDescription);
		pass->depthAttachmentReference = depthAttachmentReference;
		pass->clearDepthValue = {1.0f, 0};
	}

	void PassLogic::CreateResolveAttachment(Context *context,
											std::shared_ptr<Pass> pass,
											VkImageLayout initLayout, VkImageLayout finalLayout)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto surfaceFormat = global->surfaceFormat;

		VkAttachmentDescription resolveAttachmentDescription = {};
		resolveAttachmentDescription.format = surfaceFormat.format;
		resolveAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		resolveAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		resolveAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		resolveAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		resolveAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		resolveAttachmentDescription.initialLayout = initLayout;
		resolveAttachmentDescription.finalLayout = finalLayout;

		VkAttachmentReference resolveAttachmentReference = {};
		resolveAttachmentReference.attachment = 2;
		resolveAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		pass->attachmentDescriptions.push_back(resolveAttachmentDescription);
		pass->resolveAttachmentReference = resolveAttachmentReference;
	}

	void PassLogic::GetSwapchainImage2ds(Context *context,
										 std::shared_ptr<Pass> pass)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto swapchainImageCount = global->swapchainImageCount;
		auto &swapchainImages = global->swapchainImages;

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			pass->colorImage2ds.push_back(swapchainImages[i]);
		}
		pass->isGetSwapchainImage = true;
	}

	void PassLogic::CreateColorImage2d(Context *context,
										std::shared_ptr<Pass> pass,
										VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto surfaceFormat = global->surfaceFormat;
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto swapchainImageCount = global->swapchainImageCount;

		ImageCreateInfo imageCreateInfo = {
			surfaceFormat.format, {currentExtent.width, currentExtent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			1,
			samplers,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
		auto colorImage2d = ImageLogic::CreateByInfo(context,
													 imageCreateInfo);

		pass->colorImage2ds.push_back(colorImage2d);
	}

	void PassLogic::CreateDepthImage2d(Context *context,
									   std::shared_ptr<Pass> pass,
									   VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto depthFormat = global->depthFormat;

		ImageCreateInfo imageCreateInfo = {
			depthFormat, {currentExtent.width, currentExtent.height, 0}, VK_IMAGE_ASPECT_DEPTH_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			1,
			samplers,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL};

		auto depthImage2d = ImageLogic::CreateByInfo(context,
													 imageCreateInfo);

		pass->depthImage2d = depthImage2d;
	}

	void PassLogic::CreateResolveImage2d(Context *context,
										 std::shared_ptr<Pass> pass,
										 VkSampleCountFlagBits samplers)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto surfaceFormat = global->surfaceFormat;
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		ImageCreateInfo imageCreateInfo = {
			surfaceFormat.format, {currentExtent.width, currentExtent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			1,
			samplers,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};

		auto resolveImage2d = ImageLogic::CreateByInfo(context,
													   imageCreateInfo);

		pass->resolveImage2d = resolveImage2d;
	}

	void PassLogic::DestroyColorImage2ds(Context *context,
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

	void PassLogic::DestroyDepthImage2d(Context *context,
										std::shared_ptr<Pass> pass)
	{
		if (pass->depthImage2d != nullptr)
			ImageLogic::Destroy(context, pass->depthImage2d);
	}

	void PassLogic::DestroyResolveImage2d(Context *context,
										  std::shared_ptr<Pass> pass)
	{
		if (pass->resolveImage2d != nullptr)
			ImageLogic::Destroy(context, pass->resolveImage2d);
	}

	void PassLogic::AddSubpassDependency(Context *context,
										 std::shared_ptr<Pass> pass,
										 uint32_t srcSubpass, uint32_t dstSubpass,
										 VkPipelineStageFlags srcStageMask, VkAccessFlags srcAccessMask,
										 VkPipelineStageFlags dstStageMask, VkAccessFlags dstAccessMask)
	{
		VkSubpassDependency subpassDependency = {};
		subpassDependency.srcSubpass = srcSubpass;
		subpassDependency.dstSubpass = dstSubpass;
		subpassDependency.srcStageMask = srcStageMask;
		subpassDependency.srcAccessMask = srcAccessMask;
		subpassDependency.dstStageMask = dstStageMask;
		subpassDependency.dstAccessMask = dstAccessMask;
		// subpassDependency.dependencyFlags = dependencyFlags;

		pass->subpassDependencies.push_back(subpassDependency);
	}

	void PassLogic::SetSubPassDescription(Context *context,
										  std::shared_ptr<Pass> pass)
	{
		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		if (!pass->colorImage2ds.empty())
		{
			subpassDescription.colorAttachmentCount = 1;
			subpassDescription.pColorAttachments = &pass->colorAttachmentReference;
		}

		if (pass->depthImage2d != nullptr)
		{
			subpassDescription.pDepthStencilAttachment = &pass->depthAttachmentReference;
		}

		if (pass->resolveImage2d != nullptr)
		{
			subpassDescription.pResolveAttachments = &pass->resolveAttachmentReference;
		}

		pass->subpassDescription = subpassDescription;
	}

	void PassLogic::Create(Context *context,
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

	void PassLogic::Destroy(Context *context,
							std::shared_ptr<Pass> pass)
	{
		auto &renderPass = pass->renderPass;
		vkDestroyRenderPass(LogicalDeviceLogic::Get(context),
							renderPass, nullptr);
	}
}
