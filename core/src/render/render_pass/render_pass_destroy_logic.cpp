
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
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
