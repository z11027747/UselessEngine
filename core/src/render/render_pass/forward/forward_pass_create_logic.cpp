﻿
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	std::shared_ptr<Pass> RenderPassLogic::CreateForward(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto surfaceFormat = global->surfaceFormat.format;
		auto msaaSamples = global->msaaSamples;

		auto hasMSAA = (msaaSamples != VK_SAMPLE_COUNT_1_BIT);

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::Forward;
		pass->extent = {currentExtent.width, currentExtent.height};

		// image2ds
		FramebufferLogic::CreateColorImage2d(context, pass, msaaSamples);
		FramebufferLogic::CreateDepthImage2d(context, pass, msaaSamples);
		FramebufferLogic::CreateResolveImage2d(context, pass, VK_SAMPLE_COUNT_1_BIT);

		// count: 1
		PassLogic::SetSubpassCount(context, pass, 1);

		//  attachments
		//  attachment0: color
		PassLogic::CreateColorAttachment(context, pass, 0,
										 surfaceFormat, msaaSamples,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.1921569f, 0.3019608f, 0.4745098f, 1.0f});
		// attachment1: depth
		PassLogic::CreateDepthAttachment(context, pass, 0,
										 msaaSamples);
		// attachment2: resolve
		PassLogic::CreateResolveAttachment(context, pass, 0,
										   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										   {0.1921569f, 0.3019608f, 0.4745098f, 1.0f});

		// subpass0
		PassLogic::SetColorAttachment(context, pass, 0,
									  0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetDepthAttachment(context, pass, 0,
									  1);
		PassLogic::SetResolveAttachment(context, pass, 0,
										2);
		// description1
		PassLogic::SetSubpassDescription(context, pass, 0);

		// dependency: external->0
		PassLogic::AddSubpassDependency(context, pass,
										VK_SUBPASS_EXTERNAL, 0,
										VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_ACCESS_NONE,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		// dependency: 0->external
		PassLogic::AddSubpassDependency(context, pass,
										0, VK_SUBPASS_EXTERNAL,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_NONE);

		PassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		global->passMap[pass->name] = pass;
		return pass;
	}
}
