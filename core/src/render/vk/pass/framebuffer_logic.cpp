
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/cmd/cmd_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "context.h"

namespace Render
{
	void FramebufferLogic::Create(Context *context,
								  std::shared_ptr<Pass> pass)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
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
										   uint32_t imageIndex, VkCommandBuffer &vkCmdBuffer,
										   std::shared_ptr<Pass> pass)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto &surfaceCapabilities = global->surfaceCapabilities;

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
										 uint32_t imageIndex, VkCommandBuffer &vkCmdBuffer)
	{
		vkCmdEndRenderPass(vkCmdBuffer);
	}

	void FramebufferLogic::RenderUnits(Context *context,
									   uint32_t imageIndex, VkCommandBuffer &vkCmdBuffer,
									   GlobalUBO &globalUBO, bool isShadow)
	{
		auto &unitEOs = context->renderUnitEOs;
		for (const auto &unitEO : unitEOs)
		{
			if (!unitEO->active)
				continue;

			auto unit = unitEO->GetComponent<Render::Unit>();

			if (isShadow && !unit->castShadow)
				continue;

			auto pipelineName = !isShadow ? unit->pipelineName : "shadow";

			auto &graphicsPipeline = context->renderPipelines[pipelineName];
			auto &pipeline = graphicsPipeline->pipeline;
			auto &pipelineLayout = graphicsPipeline->pipelineLayout;

			vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

			auto &vertexBuffer = unit->vertexBuffer;
			VkBuffer vertexBuffers[] = {vertexBuffer->vkBuffer};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(vkCmdBuffer, 0, 1, vertexBuffers, offsets);

			auto &indices = unit->indices;
			auto &indexBuffer = unit->indexBuffer;
			vkCmdBindIndexBuffer(vkCmdBuffer, indexBuffer->vkBuffer, 0, VK_INDEX_TYPE_UINT16);

			auto &globalDescriptor = graphicsPipeline->globalDescriptors[imageIndex];
			auto &globalBuffer = graphicsPipeline->globalBuffers[imageIndex];

			BufferSetLogic::Set(context,
								globalBuffer,
								globalUBO);

			ShaderLogic::UpdateUnitDescriptor(context,
											  unit, imageIndex);

			auto unitTransform = unitEO->GetComponent<Logic::Transform>();
			auto &model = unitTransform->model;
			vkCmdPushConstants(vkCmdBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &model);

			std::vector<VkDescriptorSet> descriptorSets;
			descriptorSets.push_back(globalDescriptor->set);

			if (!isShadow)
			{
				if (unit->descriptor != nullptr)
					descriptorSets.push_back(unit->descriptor->set);
			}

			vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
									pipelineLayout, 0, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);

			vkCmdDrawIndexed(vkCmdBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		}
	}

}
