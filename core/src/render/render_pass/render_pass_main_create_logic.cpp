
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	std::shared_ptr<Pass> RenderPassLogic::CreateMain(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto msaaSamples = global->msaaSamples;

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::Main;
		pass->extent = {currentExtent.width, currentExtent.height};
		pass->msaaSamples = msaaSamples;

		PassLogic::CreateColorImage2d(context, pass, msaaSamples);
		PassLogic::CreateDepthImage2d(context, pass, msaaSamples);

		PassLogic::CreateColorAttachment(context,
										 pass, msaaSamples,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::CreateDepthAttachment(context, pass, msaaSamples);

		pass->clearColorValue = {0.1921569f, 0.3019608f, 0.4745098f, 0.0f};

		if (msaaSamples != VK_SAMPLE_COUNT_1_BIT)
		{
			PassLogic::CreateResolveImage2d(context, pass, VK_SAMPLE_COUNT_1_BIT);
			PassLogic::CreateResolveAttachment(context,
											   pass, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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
}
