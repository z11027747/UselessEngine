
#include <array>
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/device/physical_device_system.h"
#include "render/global/swapchain/swapchain_system.h"
#include "render/global/pass/pass_system.h"
#include "context.h"

namespace Render {

	void PassSystem::Create(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto logicalDevice = global->logicalDevice;

		VkAttachmentDescription colorAttachmentDescription = {};
		VkAttachmentReference colorAttachmentReference = {};
		MakeColorAttachment(context,
			colorAttachmentDescription,
			colorAttachmentReference);

		VkAttachmentDescription depthAttachmentDescription = {};
		VkAttachmentReference depthAttachmentReference = {};
		MakeDepthAttachment(context,
			depthAttachmentDescription,
			depthAttachmentReference);

		std::array<VkAttachmentDescription, 2> attachments = {
			colorAttachmentDescription,
			depthAttachmentDescription
		};

		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentReference;
		subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpassDescription;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask =
			VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;

		VkRenderPass vkRenderPass;
		auto ret = vkCreateRenderPass(logicalDevice, &createInfo, nullptr, &vkRenderPass);

		global->renderPass = vkRenderPass;
	}

	void PassSystem::Destroy(Context* context) {
		auto& renderEO = context->renderEO;

		auto global = renderEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& renderPass = global->renderPass;

		vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
	}

	void PassSystem::MakeColorAttachment(Context* context,
		VkAttachmentDescription& colorAttachmentDescription,
		VkAttachmentReference& colorAttachmentReference
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto swapchianFormat = global->surfaceFormat.format;

		colorAttachmentDescription.format = swapchianFormat;
		colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		colorAttachmentReference.attachment = 0; //index
		colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	void PassSystem::MakeDepthAttachment(Context* context,
		VkAttachmentDescription& depthAttachmentDescription,
		VkAttachmentReference& depthAttachmentReference
	) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto depthImageFormat = global->depthImageFormat;

		depthAttachmentDescription.format = depthImageFormat;
		depthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;

		depthAttachmentReference.attachment = 1; //index
		depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

}
