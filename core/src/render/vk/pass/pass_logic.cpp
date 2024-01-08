
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
	std::shared_ptr<Pass> PassLogic::CreateImGui(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto pass = std::make_shared<Pass>();
		pass->name = Pass_ImGui;

		RenderPassLogic::CreateColorAttachment(context, pass,
											   VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		RenderPassLogic::GetSwapchainImage2ds(context, pass);
		RenderPassLogic::AddSubpassDependency(context, pass,
											  VK_SUBPASS_EXTERNAL, 0,
											  VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
											  VK_ACCESS_MEMORY_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		RenderPassLogic::SetSubPassDescription(context, pass);
		RenderPassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		global->passes.emplace(pass->name, pass);
		return pass;
	}

	std::shared_ptr<Pass> PassLogic::CreateMain(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto pass = std::make_shared<Pass>();
		pass->name = Pass_Main;

		RenderPassLogic::CreateColorAttachment(context, pass,
											   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		RenderPassLogic::CreateDepthAttachment(context, pass);
		RenderPassLogic::CreateColorImage2ds(context, pass);
		RenderPassLogic::CreateDepthImage2ds(context, pass);
		RenderPassLogic::AddSubpassDependency(context, pass,
											  VK_SUBPASS_EXTERNAL, 0,
											  VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
											  VK_ACCESS_MEMORY_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		RenderPassLogic::SetSubPassDescription(context, pass);
		RenderPassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		global->passes.emplace(pass->name, pass);
		return pass;
	}

	std::shared_ptr<Pass> PassLogic::CreateShadow(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		auto pass = std::make_shared<Pass>();
		pass->name = Pass_Shadow;

		RenderPassLogic::CreateDepthAttachment(context, pass, 0);
		RenderPassLogic::CreateDepthImage2ds(context, pass);
		RenderPassLogic::AddSubpassDependency(context, pass,
											  VK_SUBPASS_EXTERNAL, 0,
											  VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
											  VK_ACCESS_MEMORY_READ_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
		RenderPassLogic::SetSubPassDescription(context, pass);
		RenderPassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		global->passes.emplace(pass->name, pass);
		return pass;
	}

	void PassLogic::DestroyAll(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		for (const auto &kv : global->passes)
		{
			auto &pass = kv.second;
			RenderPassLogic::DestroyColorImage2ds(context, pass);
			RenderPassLogic::DestroyDepthImage2ds(context, pass);
			RenderPassLogic::Destroy(context, pass);
			FramebufferLogic::Destroy(context, pass);
		}
	}
}
