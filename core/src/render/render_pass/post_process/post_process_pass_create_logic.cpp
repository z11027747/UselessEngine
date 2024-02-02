
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	std::shared_ptr<Pass> RenderPassLogic::CreatePostProcess(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::PostProcess;
		pass->extent = {currentExtent.width, currentExtent.height};

		// image2ds
		FramebufferLogic::CreateColorImage2d(context, pass, VK_SAMPLE_COUNT_1_BIT);
		FramebufferLogic::CreateBlitImage2d(context, pass, 4u);
		FramebufferLogic::CreateInputImage2d(context, pass);
		FramebufferLogic::CreateInputImage2d(context, pass);

		// subpass count: 3
		PassLogic::SetSubpassCount(context, pass, 3);

		// subpass0: toon mapping
		// attachment0: color
		PassLogic::CreateColorAttachment(context, pass, 0,
										 VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		// description1
		PassLogic::SetSubpassDescription(context, pass, 0);

		// subpass1: bloom
		// attachment0: color
		PassLogic::CreateColorAttachment(context, pass, 1,
										 VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		// description1
		PassLogic::SetSubpassDescription(context, pass, 1);

		// subpass2
		// attachment0: color
		PassLogic::CreateColorAttachment(context, pass, 2,
										 VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		// attachment1: input-toon mapping
		PassLogic::CreateInputAttachment(context, pass, 2,
										 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, {1.0f, 1.0f, 1.0f, 1.0f});
		// attachment2: input-bloom
		PassLogic::CreateInputAttachment(context, pass, 2,
										 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, {0.0f, 0.0f, 0.0f, 0.0f});
		// description0
		PassLogic::SetSubpassDescription(context, pass, 2);

		// dependency: 0->1
		PassLogic::AddSubpassDependency(context, pass,
										0, 1,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
		// dependency: 1->2
		PassLogic::AddSubpassDependency(context, pass,
										1, 2,
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
