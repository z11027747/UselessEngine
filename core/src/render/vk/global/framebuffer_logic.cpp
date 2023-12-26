
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <array>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/pass_logic.h"
#include "render/vk/global/framebuffer_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/unit/unit_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "context.h"

namespace Render {

	void FramebufferLogic::Create(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& surfaceCapabilities = global->surfaceCapabilities;
		auto& renderPass = global->renderPass;

		auto& swapchainColorImage2Ds = global->swapchainColorImage2Ds;
		auto& swapchainDepthImage2Ds = global->swapchainDepthImage2Ds;
		auto imageCount = static_cast<uint32_t>(swapchainColorImage2Ds.size());

		auto& frameBuffers = global->frameBuffers;
		frameBuffers.resize(imageCount);

		for (auto i = 0u; i < imageCount; i++) {

			std::array<VkImageView, 2> attachments = {
				swapchainColorImage2Ds[i]->vkImageView,
				swapchainDepthImage2Ds[i]->vkImageView
			};

			VkFramebufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.width = surfaceCapabilities.currentExtent.width;
			createInfo.height = surfaceCapabilities.currentExtent.height;
			createInfo.renderPass = renderPass;
			createInfo.layers = 1;

			auto ret = vkCreateFramebuffer(logicalDevice, &createInfo, nullptr, &frameBuffers[i]);
			CheckRet(ret, "vkCreateFramebuffer");
		}

		global->maxFrameInFlight = imageCount;

		CreateFences(context);
		CreateSemaphores(context);
		AllocateCmdBuffers(context);
	}

	void FramebufferLogic::Destroy(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& frameBuffers = global->frameBuffers;

		for (auto& frameBuffer : frameBuffers) {
			vkDestroyFramebuffer(logicalDevice, frameBuffer, nullptr);
		}

		DestroyFences(context);
		DestroySemaphores(context);
	}

	void FramebufferLogic::CreateFences(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& inFlightFences = global->inFlightFences;
		inFlightFences.resize(maxFrameInFlight);

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			VkFenceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			VkFence vkFence;
			auto ret = vkCreateFence(logicalDevice, &createInfo, nullptr, &vkFence);
			CheckRet(ret, "vkCreateFence");

			inFlightFences[i] = vkFence;
		}
	}

	void FramebufferLogic::DestroyFences(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto& inFlightFences = global->inFlightFences;
		for (const auto& inFlightFence : inFlightFences) {
			vkDestroyFence(logicalDevice, inFlightFence, nullptr);
		}
		inFlightFences.clear();
	}

	void FramebufferLogic::CreateSemaphores(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& imageAvailableSemaphores = global->imageAvailableSemaphores;
		auto& renderFinishedSemaphores = global->renderFinishedSemaphores;
		imageAvailableSemaphores.resize(maxFrameInFlight);
		renderFinishedSemaphores.resize(maxFrameInFlight);

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			VkSemaphoreCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			createInfo.flags = 0;

			VkSemaphore availableSemaphore;
			auto avaliableRet = vkCreateSemaphore(logicalDevice, &createInfo, nullptr, &availableSemaphore);
			CheckRet(avaliableRet, "vkCreateSemaphore-Avaliable");

			VkSemaphore finishedSemaphore;
			auto finishedRet = vkCreateSemaphore(logicalDevice, &createInfo, nullptr, &finishedSemaphore);
			CheckRet(finishedRet, "vkCreateSemaphore-Finished");

			imageAvailableSemaphores[i] = availableSemaphore;
			renderFinishedSemaphores[i] = finishedSemaphore;
		}
	}

	void FramebufferLogic::DestroySemaphores(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		auto& imageAvailableSemaphores = global->imageAvailableSemaphores;
		auto& renderFinishedSemaphores = global->renderFinishedSemaphores;

		for (auto i = 0u; i < maxFrameInFlight; i++) {
			vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
		}

		imageAvailableSemaphores.clear();
		renderFinishedSemaphores.clear();
	}

	void FramebufferLogic::AllocateCmdBuffers(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto maxFrameInFlight = global->maxFrameInFlight;

		global->cmdBuffers = CmdPoolLogic::AllocateBuffers(context,
			maxFrameInFlight, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	}

	void FramebufferLogic::Update(Context* context) {
		UpdateWaitFence(context);

		uint32_t imageIndex;
		UpdateAcquireNext(context, imageIndex);
		UpdateSubmitCmds(context, imageIndex);
		UpdatePresent(context, imageIndex);
	}

	bool FramebufferLogic::UpdateWaitFence(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;

		auto currFrame = global->currFrame;
		auto& inFlightFence = global->inFlightFences[currFrame];;

		auto waitFenceRet = vkWaitForFences(logicalDevice, 1, &inFlightFence, true, UINT64_MAX);
		CheckRet(waitFenceRet, "vkWaitForFences");

		vkResetFences(logicalDevice, 1, &inFlightFence);
		return true;
	}

	bool FramebufferLogic::UpdateAcquireNext(Context* context, uint32_t& imageIndex) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalDevice = global->logicalDevice;
		auto& swapchain = global->swapchain;

		auto& frameBuffers = global->frameBuffers;
		auto& cmdBuffers = global->cmdBuffers;

		auto& currFrame = global->currFrame;
		auto& imageAvailableSemaphore = global->imageAvailableSemaphores[currFrame];

		auto acquireNextRet = vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX,
			imageAvailableSemaphore, nullptr, &imageIndex);
		CheckRet(acquireNextRet, "vkAcquireNextImageKHR");

		return true;
	}

	bool FramebufferLogic::UpdateDraw(Context* context, uint32_t imageIndex) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalQueue = global->logicalQueue;
		auto& swapchain = global->swapchain;
		auto& renderPass = global->renderPass;
		auto& surfaceCapabilities = global->surfaceCapabilities;

		auto& frameBuffer = global->frameBuffers[imageIndex];
		auto& cmdBuffer = global->cmdBuffers[imageIndex];

		auto& currFrame = global->currFrame;
		auto& inFlightFence = global->inFlightFences[currFrame];
		auto& imageAvailableSemaphore = global->imageAvailableSemaphores[currFrame];
		auto& renderFinishedSemaphore = global->renderFinishedSemaphores[currFrame];

		auto& cameraEO = context->cameraEO;
		auto camera = cameraEO->GetComponent<Logic::Camera>();

		auto& pipelineEO = context->renderPipelineEOs["test"];
		auto graphicsPipeline = pipelineEO->GetComponent<GraphicsPipeline>();
		auto& pipeline = graphicsPipeline->pipeline;
		auto& pipelineLayout = graphicsPipeline->pipelineLayout;

		UniformCameraTransform uct = {
			camera->view,
			camera->projection
		};

		ShaderLogic::UpdateUniform0<UniformTest, UniformCameraTransform>(context,
			graphicsPipeline,
			imageIndex, uct);

		CmdPoolLogic::ResetBuffer(cmdBuffer, 0);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		auto beginRet = vkBeginCommandBuffer(cmdBuffer, &beginInfo);
		CheckRet(beginRet, "vkBeginCommandBuffer");

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.framebuffer = frameBuffer;
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = surfaceCapabilities.currentExtent;

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassBeginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		auto& uniform = graphicsPipeline->uniforms[imageIndex];

		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

		auto& renderUnitEOs = context->renderUnitEOs;
		for (const auto& renderUnitEO : renderUnitEOs) {

			auto transform = renderUnitEO->GetComponent<Logic::Transform>();

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform->position);
			model = glm::rotate(model, glm::radians(transform->eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform->eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform->eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, transform->scale);

			UniformObjectTransform uct = {
				model
			};

			ShaderLogic::UpdateUniform1<UniformTest, UniformObjectTransform>(context,
				graphicsPipeline,
				imageIndex, uct);

			auto unit = renderUnitEO->GetComponent<Render::Unit>();

			auto& vertexBuffer = unit->vertexBuffer;
			VkBuffer vertexBuffers[] = { vertexBuffer->vkBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);

			auto& indices = unit->indices;
			auto& indexBuffer = unit->indexBuffer;
			vkCmdBindIndexBuffer(cmdBuffer, indexBuffer->vkBuffer, 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipelineLayout, 0, 1, &uniform->descriptorSet, 0, nullptr);

			vkCmdDrawIndexed(cmdBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(cmdBuffer);

		auto endRet = vkEndCommandBuffer(cmdBuffer);
		CheckRet(endRet, "vkEndCommandBuffer");

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
		VkPipelineStageFlags waitStages[] = {
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		};
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

		auto submitRet = vkQueueSubmit(logicalQueue, 1, &submitInfo, inFlightFence);
		CheckRet(submitRet, "vkQueueSubmit");

		return true;
	}

	void FramebufferLogic::UpdatePresent(Context* context, uint32_t imageIndex) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Global>();
		auto& logicalQueue = global->logicalQueue;
		auto& swapchain = global->swapchain;

		auto& currFrame = global->currFrame;
		auto maxFrameInFlight = global->maxFrameInFlight;
		auto& renderFinishedSemaphore = global->renderFinishedSemaphores[currFrame];

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderFinishedSemaphore;

		auto presentRet = vkQueuePresentKHR(logicalQueue, &presentInfo);
		CheckRet(presentRet, "vkQueuePresentKHR");

		currFrame = (currFrame + 1) % maxFrameInFlight;
	}

}
