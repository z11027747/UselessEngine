
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "context.h"

namespace Render
{
	void PassLogic::CreateImGui(Context *context)
	{
		auto pass = std::make_shared<Pass>();
		pass->name = "imGui";

		RenderPassLogic::CreateColorAttachment(context, pass,
											   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		RenderPassLogic::GetSwapchainImage2ds(context, pass);
		RenderPassLogic::AddSubpassDependency(context, pass,
											  VK_SUBPASS_EXTERNAL, 0,
											  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
											  VK_ACCESS_NONE, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		RenderPassLogic::SetSubPassDescription(context, pass);
		RenderPassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		context->renderImGuiPass = pass;
	}

	void PassLogic::CreateMain(Context *context)
	{
		auto pass = std::make_shared<Pass>();
		pass->name = "main";

		RenderPassLogic::CreateColorAttachment(context, pass,
											   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		RenderPassLogic::CreateDepthAttachment(context, pass);
		RenderPassLogic::CreateColorImage2ds(context, pass);
		RenderPassLogic::CreateDepthImage2ds(context, pass);
		RenderPassLogic::AddSubpassDependency(context, pass,
											  VK_SUBPASS_EXTERNAL, 0,
											  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
											  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_NONE);
		RenderPassLogic::SetSubPassDescription(context, pass);
		RenderPassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		context->renderMainPass = pass;
	}

	void PassLogic::CreateShadow(Context *context)
	{
		auto pass = std::make_shared<Pass>();
		pass->name = "shadow";

		// RenderPassLogic::CreateDepthAttachment(context, pass, 0);
		// RenderPassLogic::CreateDepthImage2ds(context, pass);
		// RenderPassLogic::AddSubpassDependency(context, pass,
		// 									  VK_SUBPASS_EXTERNAL, 0,
		// 									  VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
		// 									  VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
		// RenderPassLogic::SetSubPassDescription(context, pass);
		// RenderPassLogic::Create(context, pass);
		// FramebufferLogic::Create(context, pass);

		RenderPassLogic::CreateColorAttachment(context, pass,
											   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		RenderPassLogic::CreateDepthAttachment(context, pass);
		RenderPassLogic::CreateColorImage2ds(context, pass);
		RenderPassLogic::CreateDepthImage2ds(context, pass);
		RenderPassLogic::AddSubpassDependency(context, pass,
											  VK_SUBPASS_EXTERNAL, 0,
											  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
											  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_NONE);
		RenderPassLogic::SetSubPassDescription(context, pass);
		RenderPassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		context->renderShadowPass = pass;
	}

	void PassLogic::DestroyAll(Context *context)
	{
		std::vector<std::shared_ptr<Pass>> passes = {
			context->renderImGuiPass,
			context->renderMainPass,
			context->renderShadowPass};

		for (const auto &pass : passes)
		{
			RenderPassLogic::DestroyColorImage2ds(context, pass);
			RenderPassLogic::DestroyDepthImage2ds(context, pass);
			RenderPassLogic::Destroy(context, pass);
			FramebufferLogic::Destroy(context, pass);
		}
	}
}
