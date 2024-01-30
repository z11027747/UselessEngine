﻿
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

		PassLogic::GetSwapchainImage2ds(context, pass);

		PassLogic::CreateColorAttachment(context, pass,
										 VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

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
}
