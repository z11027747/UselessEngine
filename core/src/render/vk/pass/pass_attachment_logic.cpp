
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void PassLogic::CreateColorAttachment(Context *context,
										  std::shared_ptr<Pass> pass, uint32_t subpassIndex,
										  VkFormat format, VkSampleCountFlagBits samplers,
										  VkImageLayout initLayout, VkImageLayout finalLayout,
										  VkClearColorValue &&clearColorValue)
	{
		auto &subpass = pass->subpasses[subpassIndex];
		subpass->msaaSamples = samplers;

		VkAttachmentDescription colorAttachmentDescription = {};
		colorAttachmentDescription.format = format;
		colorAttachmentDescription.samples = samplers;
		colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentDescription.initialLayout = initLayout;
		colorAttachmentDescription.finalLayout = finalLayout;

		pass->attachmentDescriptions.push_back(colorAttachmentDescription);

		VkClearValue clearValue = {};
		clearValue.color = clearColorValue;
		pass->clearValues.push_back(clearValue);
	}
	void PassLogic::SetColorAttachment(Context *context,
									   std::shared_ptr<Pass> pass, uint32_t subpassIndex,
									   uint32_t index, VkImageLayout subPassLayout)
	{
		auto &subpass = pass->subpasses[subpassIndex];

		VkAttachmentReference colorAttachmentReference = {};
		colorAttachmentReference.attachment = index;
		colorAttachmentReference.layout = subPassLayout;

		subpass->colorAttachmentReferences.push_back(colorAttachmentReference);
	}
	void PassLogic::SetInputAttachment(Context *context,
									   std::shared_ptr<Pass> pass, uint32_t subpassIndex,
									   uint32_t index, VkImageLayout subPassLayout)
	{
		auto &subpass = pass->subpasses[subpassIndex];

		VkAttachmentReference inputAttachmentReference = {};
		inputAttachmentReference.attachment = index;
		inputAttachmentReference.layout = subPassLayout;

		subpass->inputAttachmentReferences.push_back(inputAttachmentReference);
	}

	void PassLogic::CreateDepthAttachment(Context *context,
										  std::shared_ptr<Pass> pass, uint32_t subpassIndex,
										  VkSampleCountFlagBits samplers)
	{
		auto &subpass = pass->subpasses[subpassIndex];

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

		pass->attachmentDescriptions.push_back(depthAttachmentDescription);

		VkClearValue clearValue = {};
		clearValue.depthStencil = {1.0f, 0};
		pass->clearValues.push_back(clearValue);
	}
	void PassLogic::SetDepthAttachment(Context *context,
									   std::shared_ptr<Pass> pass, uint32_t subpassIndex,
									   uint32_t index)
	{
		auto &subpass = pass->subpasses[subpassIndex];

		VkAttachmentReference depthAttachmentReference = {};
		depthAttachmentReference.attachment = index;
		depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		subpass->depthAttachmentReference = depthAttachmentReference;
	}

	void PassLogic::CreateResolveAttachment(Context *context,
											std::shared_ptr<Pass> pass, uint32_t subpassIndex,
											VkImageLayout initLayout, VkImageLayout finalLayout)
	{
		auto &subpass = pass->subpasses[subpassIndex];

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

		pass->attachmentDescriptions.push_back(resolveAttachmentDescription);

		VkClearValue clearValue = {};
		clearValue.color = {0.0f, 0.0f, 0.0f, 0.0f};
		pass->clearValues.push_back(clearValue);
	}

	void PassLogic::SetResolveAttachment(Context *context,
											std::shared_ptr<Pass> pass, uint32_t subpassIndex,
											uint32_t index)
	{
		auto &subpass = pass->subpasses[subpassIndex];

		VkAttachmentReference resolveAttachmentReference = {};
		resolveAttachmentReference.attachment = index;
		resolveAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		subpass->resolveAttachmentReference = resolveAttachmentReference;
	}
}
