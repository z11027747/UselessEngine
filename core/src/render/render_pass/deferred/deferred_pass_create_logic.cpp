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
	std::shared_ptr<Pass> RenderPassLogic::CreateDeferred(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::Deferred;
		pass->extent = {currentExtent.width, currentExtent.height};

		auto samplers = VK_SAMPLE_COUNT_1_BIT;

		// images
		FramebufferLogic::CreateColorImage2d(context, pass, samplers);
		FramebufferLogic::CreateDepthImage2d(context, pass, samplers);
		FramebufferLogic::CreateInputImage2d(context, pass);
		FramebufferLogic::CreateInputImage2d(context, pass);
		FramebufferLogic::CreateInputImage2d(context, pass);
		FramebufferLogic::CreateInputImage2d(context, pass);

		// subpass count: 2
		PassLogic::SetSubpassCount(context, pass, 2);

		// attachments
		// attachment0: color
		PassLogic::CreateColorAttachment(context, pass, 1,
										 samplers,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.1921569f, 0.3019608f, 0.4745098f, 0.0f});
		// attachment1: depth
		PassLogic::CreateDepthAttachment(context, pass, 0,
										 samplers);
		// attachment2: gbuffer-position
		PassLogic::CreateColorAttachment(context, pass, 0,
										 samplers,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		// attachment3: gbuffer-normal
		PassLogic::CreateColorAttachment(context, pass, 0,
										 samplers,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		// attachment4: gbuffer-color
		PassLogic::CreateColorAttachment(context, pass, 0,
										 samplers,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		// attachment5: gbuffer-material
		PassLogic::CreateColorAttachment(context, pass, 0,
										 samplers,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// subpass0: GeometryPass
		PassLogic::SetColorAttachment(context, pass, 0,
									  2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetColorAttachment(context, pass, 0,
									  3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetColorAttachment(context, pass, 0,
									  4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetColorAttachment(context, pass, 0,
									  5, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetDepthAttachment(context, pass, 0,
									  1);
		// descriptions
		PassLogic::SetSubpassDescription(context, pass, 0);

		// subpass1: LightingPass
		PassLogic::SetColorAttachment(context, pass, 1,
									  0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 1,
									  2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, {0.0f, 0.0f, 0.0f, 0.0f});
		PassLogic::SetInputAttachment(context, pass, 1,
									  3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, {0.5f, 0.5f, 0.0f, 0.0f});
		PassLogic::SetInputAttachment(context, pass, 1,
									  4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, {0.0f, 0.0f, 0.0f, 0.0f});
		PassLogic::SetInputAttachment(context, pass, 1,
									  5, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, {0.0f, 0.0f, 0.0f, 0.0f});
		// descriptions
		PassLogic::SetSubpassDescription(context, pass, 1);

		// dependency: 0->1
		PassLogic::AddSubpassDependency(context, pass,
										0, 1,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
		// dependency: external->0
		PassLogic::AddSubpassDependency(context, pass,
										VK_SUBPASS_EXTERNAL, 0,
										VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_MEMORY_READ_BIT,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		// dependency: 0->external
		PassLogic::AddSubpassDependency(context, pass,
										0, VK_SUBPASS_EXTERNAL,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_MEMORY_READ_BIT);
		PassLogic::Create(context, pass);

		FramebufferLogic::Create(context, pass);

		global->passMap[pass->name] = pass;
		return pass;
	}
}
