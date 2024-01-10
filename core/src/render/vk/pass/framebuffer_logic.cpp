
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/unit/unit_comp.h"
#include "context.h"

namespace Render
{
	void FramebufferLogic::Create(Context *context,
								  std::shared_ptr<Pass> pass)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &surfaceCapabilities = global->surfaceCapabilities;
		auto swapchainImageCount = global->swapchainImageCount;

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			std::vector<VkImageView> attachments = {};
			if (pass->colorImage2ds.size() > 0)
				attachments.push_back(pass->colorImage2ds[i]->vkImageView);

			if (pass->depthImage2ds.size() > 0)
				attachments.push_back(pass->depthImage2ds[i]->vkImageView);

			VkFramebufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.width = surfaceCapabilities.currentExtent.width;
			createInfo.height = surfaceCapabilities.currentExtent.height;
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

		std::vector<VkClearValue> clearValues;
		if (pass->colorImage2ds.size() > 0)
		{
			VkClearValue colorValue = {};
			colorValue.color = pass->clearColorValue;
			clearValues.push_back(colorValue);
		}
		if (pass->depthImage2ds.size() > 0)
		{
			VkClearValue depthValue = {};
			depthValue.depthStencil = pass->clearDepthValue;
			clearValues.push_back(depthValue);
		}

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.framebuffer = frameBuffer;
		renderPassBeginInfo.renderArea.offset = {0, 0};
		renderPassBeginInfo.renderArea.extent = surfaceCapabilities.currentExtent;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassBeginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(vkCmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void FramebufferLogic::EndRenderPass(Context *context,
										 uint32_t imageIndex)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

		vkCmdEndRenderPass(vkCmdBuffer);
	}
}