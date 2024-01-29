
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	std::shared_ptr<Pass> RenderPassLogic::CreateImGui(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::ImGui;
		pass->extent = {currentExtent.width, currentExtent.height};

		PassLogic::CreateColorAttachment(context, pass,
										 VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		PassLogic::GetSwapchainImage2ds(context, pass);

		PassLogic::AddSubpassDependency(context, pass,
										VK_SUBPASS_EXTERNAL, 0,
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_SHADER_READ_BIT,					  // src
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT); // dst
		PassLogic::AddSubpassDependency(context, pass,
										0, VK_SUBPASS_EXTERNAL,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, // src
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);		 // dst

		PassLogic::SetSubPassDescription(context, pass);
		PassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		global->passMap[pass->name] = pass;
		return pass;
	}

	std::shared_ptr<Pass> RenderPassLogic::CreateMain(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto msaaSamples = global->msaaSamples;

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::Main;
		pass->extent = {currentExtent.width, currentExtent.height};

		PassLogic::CreateColorAttachment(context,
										 pass, msaaSamples,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::CreateDepthAttachment(context, pass, msaaSamples);
		PassLogic::CreateColorImage2d(context, pass, msaaSamples);
		PassLogic::CreateDepthImage2d(context, pass, msaaSamples);

		pass->clearColorValue = {0.1921569f, 0.3019608f, 0.4745098f, 0.0f};

		if (msaaSamples != VK_SAMPLE_COUNT_1_BIT)
		{
			pass->msaaSamples = msaaSamples;
			PassLogic::CreateResolveAttachment(context,
											   pass, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			PassLogic::CreateResolveImage2d(context, pass, VK_SAMPLE_COUNT_1_BIT);
		}

		PassLogic::AddSubpassDependency(context, pass,
										VK_SUBPASS_EXTERNAL, 0,
										VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_MEMORY_READ_BIT,															// src
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT); // dst
		PassLogic::AddSubpassDependency(context, pass,
										0, VK_SUBPASS_EXTERNAL,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, // src
										VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_MEMORY_READ_BIT);														   // dst

		PassLogic::SetSubPassDescription(context, pass);
		PassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		global->passMap[pass->name] = pass;
		return pass;
	}

	std::shared_ptr<Pass> RenderPassLogic::CreateShadow(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		// auto shadowMapSize = 512u;

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::Shadow;
		// pass->extent = {shadowMapSize, shadowMapSize};
		pass->extent = {currentExtent.width, currentExtent.height};

		PassLogic::CreateDepthAttachment(context, pass,
										 VK_SAMPLE_COUNT_1_BIT, 0);
		PassLogic::CreateDepthImage2d(context, pass,
									  VK_SAMPLE_COUNT_1_BIT);

		PassLogic::AddSubpassDependency(context, pass,
										VK_SUBPASS_EXTERNAL, 0,
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_SHADER_READ_BIT,																		 // src
										VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT); // dst
		PassLogic::AddSubpassDependency(context, pass,
										0, VK_SUBPASS_EXTERNAL,
										VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT, // src
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_SHADER_READ_BIT);																	   // dst

		PassLogic::SetSubPassDescription(context, pass);
		PassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		global->passMap[pass->name] = pass;
		return pass;
	}

	void RenderPassLogic::DestroyAll(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		for (const auto &kv : global->passMap)
		{
			auto &pass = kv.second;
			PassLogic::DestroyColorImage2ds(context, pass);
			PassLogic::DestroyDepthImage2d(context, pass);
			PassLogic::DestroyResolveImage2d(context, pass);
			PassLogic::Destroy(context, pass);
			FramebufferLogic::Destroy(context, pass);
		}
	}
}
