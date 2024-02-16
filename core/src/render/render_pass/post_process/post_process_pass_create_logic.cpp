
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
		FramebufferLogic::CreateBlitImage2d(context, pass, 1u);
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R8G8B8A8_UNORM); // SSAO
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R8G8B8A8_UNORM); // toonmapping
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R8G8B8A8_UNORM); // gaussblur
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R8G8B8A8_UNORM); // bloom

		// subpass count: 3
		PassLogic::SetSubpassCount(context, pass, 5);

		// attachments
		//  attachment0: color
		PassLogic::CreateColorAttachment(context, pass, 4,
										 surfaceFormat.format, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.1921569f, 0.3019608f, 0.4745098f, 0.0f});
		// attachment1: SSAO
		PassLogic::CreateColorAttachment(context, pass, 0,
										 VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});
		// attachment2: toonmapping
		PassLogic::CreateColorAttachment(context, pass, 1,
										 VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {1.0f, 1.0f, 1.0f, 1.0f});
		// attachment3: gaussblur
		PassLogic::CreateColorAttachment(context, pass, 2,
										 VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});
		// attachment4: bloom
		PassLogic::CreateColorAttachment(context, pass, 3,
										 VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});

		// subpass0: SSAO
		PassLogic::SetColorAttachment(context, pass, 0,
									  1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetSubpassDescription(context, pass, 0);

		// subpass1: toonmapping
		PassLogic::SetColorAttachment(context, pass, 1,
									  2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetSubpassDescription(context, pass, 1);

		// subpass2: gaussblur
		PassLogic::SetColorAttachment(context, pass, 2,
									  3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetSubpassDescription(context, pass, 2);

		// subpass3: bloom
		PassLogic::SetColorAttachment(context, pass, 3,
									  4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 3,
									  3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetSubpassDescription(context, pass, 3);

		// subpass4: global
		PassLogic::SetColorAttachment(context, pass, 4,
									  0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 4,
									  1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 4,
									  2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 4,
									  3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 4,
									  4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetSubpassDescription(context, pass, 4);

		// dependency: 0->1
		PassLogic::AddSubpassDependency(context, pass,
										0, 1,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		// dependency: 1->2
		PassLogic::AddSubpassDependency(context, pass,
										1, 2,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
		// dependency: 2->3
		PassLogic::AddSubpassDependency(context, pass,
										2, 3,
										VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);
		// dependency: 3->4
		PassLogic::AddSubpassDependency(context, pass,
										3, 4,
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
										VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_ACCESS_NONE,
										VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_NONE);
		PassLogic::Create(context, pass);

		FramebufferLogic::Create(context, pass);

		global->passMap[pass->name] = pass;
		return pass;
	}
}
