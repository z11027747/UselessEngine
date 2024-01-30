
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

		auto mipLevels = 4u;

		auto colorImgage = global->passMap[Define::Pass::Main]->colorImage2ds[0];
		PassLogic::CreatePostProcessImage2d(context, pass,
											colorImgage, mipLevels);

		// TODO
		PassLogic::CreateColorAttachment(context, pass,
										 VK_SAMPLE_COUNT_1_BIT,
										 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		PassLogic::SetSubPassDescription(context, pass);
		PassLogic::Create(context, pass);
		FramebufferLogic::Create(context, pass);

		global->passMap[pass->name] = pass;
		return pass;
	}
}
