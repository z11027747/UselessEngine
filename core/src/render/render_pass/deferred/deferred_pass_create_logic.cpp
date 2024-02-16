
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	std::shared_ptr<Pass> RenderPassLogic::CreateDeferred(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;
		auto surfaceFormat = global->surfaceFormat.format;
		auto msaaSamples = global->msaaSamples;

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::Deferred;
		pass->extent = {currentExtent.width, currentExtent.height};

		// images
		FramebufferLogic::CreateColorImage2d(context, pass, msaaSamples);
		FramebufferLogic::CreateDepthStencilImage2d(context, pass, VK_SAMPLE_COUNT_1_BIT);
		FramebufferLogic::CreateResolveImage2d(context, pass, VK_SAMPLE_COUNT_1_BIT);
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R16G16B16A16_SFLOAT, VK_SAMPLE_COUNT_1_BIT); // position
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R16G16B16A16_SFLOAT, VK_SAMPLE_COUNT_1_BIT); // normal
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT);	   // color
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R16G16B16A16_SFLOAT, VK_SAMPLE_COUNT_1_BIT); // material
		FramebufferLogic::CreateInputImage2d(context, pass, VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT);	   // pointlight

		// subpass count: 2
		PassLogic::SetSubpassCount(context, pass, 3);

		// attachments
		// attachment0: color
		PassLogic::CreateColorAttachment(context, pass, 2,
										 surfaceFormat, msaaSamples,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.1921569f, 0.3019608f, 0.4745098f, 1.0f});
		// attachment1: depth+stencil
		PassLogic::CreateDepthStencilAttachment(context, pass, 0,
												VK_SAMPLE_COUNT_1_BIT);
		// attachment2: resolve
		PassLogic::CreateResolveAttachment(context, pass, 2,
										   VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										   {0.1921569f, 0.3019608f, 0.4745098f, 1.0f});
		// attachment3: gbuffer-position
		PassLogic::CreateColorAttachment(context, pass, 0,
										 VK_FORMAT_R16G16B16A16_SFLOAT, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});
		// attachment4: gbuffer-normal
		PassLogic::CreateColorAttachment(context, pass, 0,
										 VK_FORMAT_R16G16B16A16_SFLOAT, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});
		// attachment5: gbuffer-color
		PassLogic::CreateColorAttachment(context, pass, 0,
										 VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});
		// attachment6: gbuffer-material
		PassLogic::CreateColorAttachment(context, pass, 0,
										 VK_FORMAT_R16G16B16A16_SFLOAT, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});
		// attachment7: gbuffer-pointlight
		PassLogic::CreateColorAttachment(context, pass, 1,
										 VK_FORMAT_R8G8B8A8_UNORM, VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
										 {0.0f, 0.0f, 0.0f, 1.0f});

		// subpass0: GeometryPass
		PassLogic::SetColorAttachment(context, pass, 0,
									  3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetColorAttachment(context, pass, 0,
									  4, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetColorAttachment(context, pass, 0,
									  5, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetColorAttachment(context, pass, 0,
									  6, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetDepthAttachment(context, pass, 0,
									  1);
		// descriptions
		PassLogic::SetSubpassDescription(context, pass, 0);

		// subpass1: PointLightPass
		PassLogic::SetColorAttachment(context, pass, 1,
									  7, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetDepthAttachment(context, pass, 1,
									  1);
		PassLogic::SetInputAttachment(context, pass, 1,
									  3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 1,
									  4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 1,
									  5, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 1,
									  6, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		// descriptions
		PassLogic::SetSubpassDescription(context, pass, 1);

		// subpass2: ShadingPass
		PassLogic::SetColorAttachment(context, pass, 2,
									  0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 2,
									  3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 2,
									  4, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 2,
									  5, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 2,
									  6, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetInputAttachment(context, pass, 2,
									  7, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		PassLogic::SetResolveAttachment(context, pass, 2,
										2);
		// descriptions
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
										VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_NONE,
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
