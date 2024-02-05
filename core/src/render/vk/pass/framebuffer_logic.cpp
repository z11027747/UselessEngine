
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pass/pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void FramebufferLogic::Create(Context *context,
								  std::shared_ptr<Pass> pass)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;
		auto swapchainImageCount = global->swapchainImageCount;

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			std::vector<VkImageView> attachments = {};

			if (!pass->colorImage2ds.empty())
			{
				auto colorImageIndex = (pass->isGetSwapchainImage) ? i : 0;
				attachments.push_back(pass->colorImage2ds[colorImageIndex]->vkImageView);
			}
			if (pass->depthImage2d != nullptr)
			{
				attachments.push_back(pass->depthImage2d->vkImageView);
			}
			if (pass->resolveImage2d != nullptr)
			{
				attachments.push_back(pass->resolveImage2d->vkImageView);
			}
			for (const auto &inputImage2d : pass->inputImage2ds)
			{
				attachments.push_back(inputImage2d->vkImageView);
			}

			VkFramebufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.width = pass->extent.width;
			createInfo.height = pass->extent.height;
			createInfo.renderPass = pass->renderPass;
			createInfo.layers = 1;

			VkFramebuffer vkFramebuffer;
			auto ret = vkCreateFramebuffer(logicalDevice, &createInfo, nullptr, &vkFramebuffer);
			CheckRet(ret, "vkCreateFramebuffer");

			pass->frameBuffers.push_back(vkFramebuffer);
		}
	}

	void FramebufferLogic::Destroy(Context *context,
								   std::shared_ptr<Pass> pass)
	{
		auto &frameBuffers = pass->frameBuffers;
		for (auto &frameBuffer : frameBuffers)
		{
			vkDestroyFramebuffer(LogicalDeviceLogic::Get(context),
								 frameBuffer, nullptr);
		}
	}

	void FramebufferLogic::BeginRenderPass(Context *context,
										   uint32_t imageIndex, std::shared_ptr<Pass> pass)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &surfaceCapabilities = global->surfaceCapabilities;
		auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

		auto &renderPass = pass->renderPass;
		auto &frameBuffer = pass->frameBuffers[imageIndex];

		auto &clearValues = pass->clearValues;

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.framebuffer = frameBuffer;
		renderPassBeginInfo.renderArea.offset = pass->offset;
		renderPassBeginInfo.renderArea.extent = pass->extent;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassBeginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(vkCmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void FramebufferLogic::NextSubpass(Context *context, uint32_t imageIndex)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

		vkCmdNextSubpass(vkCmdBuffer, VK_SUBPASS_CONTENTS_INLINE);
	}

	void FramebufferLogic::EndRenderPass(Context *context, uint32_t imageIndex)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

		vkCmdEndRenderPass(vkCmdBuffer);
	}
}