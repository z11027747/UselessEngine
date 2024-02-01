
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void PassLogic::SetSubpassCount(Context *context,
									std::shared_ptr<Pass> pass, uint32_t count)
	{
		pass->subpasses.resize(count);
		pass->subpassDescriptions.resize(count);
	}

	void PassLogic::SetSubpassDescription(Context *context,
										  std::shared_ptr<Pass> pass, uint32_t subpassIndex)
	{
		auto &subpass = pass->subpasses[subpassIndex];
		auto &subpassDescription = pass->subpassDescriptions[subpassIndex];

		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		if (!pass->colorImage2ds.empty())
		{
			subpassDescription.colorAttachmentCount = static_cast<uint32_t>(subpass.inputAttachmentReferences.size());
			subpassDescription.pColorAttachments = subpass.colorAttachmentReferences.data();
		}
		if (pass->depthImage2d != nullptr)
		{
			subpassDescription.pDepthStencilAttachment = &subpass.depthAttachmentReference;
		}
		if (pass->resolveImage2d != nullptr)
		{
			subpassDescription.pResolveAttachments = &subpass.resolveAttachmentReference;
		}

		if (!subpass.inputAttachmentReferences.empty())
		{
			subpassDescription.inputAttachmentCount = static_cast<uint32_t>(subpass.inputAttachmentReferences.size());
			subpassDescription.pInputAttachments = subpass.inputAttachmentReferences.data();
		}

		pass->subpassDescriptions[subpassIndex] = subpassDescription;
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
		subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		pass->subpassDependencies.push_back(subpassDependency);
	}

	void PassLogic::Create(Context *context,
						   std::shared_ptr<Pass> pass)
	{
		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = static_cast<uint32_t>(pass->attachmentDescriptions.size());
		createInfo.pAttachments = pass->attachmentDescriptions.data();
		createInfo.subpassCount = static_cast<uint32_t>(pass->subpassDescriptions.size());
		createInfo.pSubpasses = pass->subpassDescriptions.data();
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
