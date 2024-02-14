
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	std::shared_ptr<Pass> RenderPassLogic::CreatePostProcess(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto surfaceFormat = global->surfaceFormat;

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::PostProcess;
		pass->extent = {currentExtent.width, currentExtent.height};

		// image2ds
		FramebufferLogic::CreateColorImage2d(context, pass, VK_SAMPLE_COUNT_1_BIT);
		FramebufferLogic::CreateBlitImage2d(context, pass, 4u);
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R8G8B8A8_UNORM);
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R8G8B8A8_UNORM);

		// subpass count: 3
		PassLogic::SetSubpassCount(context, pass, 3);

		// attachments
		//  attachment0: color
		PassLogic::CreateColorAttachment(context, pass, 2,
										 surfaceFormat.format, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.1921569f, 0.3019608f, 0.4745098f, 0.0f});
		// attachment1: toonmapping
		PassLogic::CreateColorAttachment(context, pass, 0,
										 VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {1.0f, 1.0f, 1.0f, 1.0f});
		// attachment2: bloom
		PassLogic::CreateColorAttachment(context, pass, 1,
										 VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});

		// subpass0: toonmapping
		PassLogic::SetColorAttachment(context, pass, 0,
									  1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		// description1
		PassLogic::SetSubpassDescription(context, pass, 0);

		// subpass1: bloom
		PassLogic::SetColorAttachment(context, pass, 1,
									  2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 1,
									  1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		// description1
		PassLogic::SetSubpassDescription(context, pass, 1);

		// subpass2
		PassLogic::SetColorAttachment(context, pass, 2,
									  0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 2,
									  1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 2,
									  2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		// description0
		PassLogic::SetSubpassDescription(context, pass, 2);

		// dependency: 0->1
		PassLogic::AddSubpassDependency(context, pass,
										0, 1,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
		// dependency: 1->2
		PassLogic::AddSubpassDependency(context, pass,
										1, 2,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
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
