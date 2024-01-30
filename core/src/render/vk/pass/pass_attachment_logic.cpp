
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
}
