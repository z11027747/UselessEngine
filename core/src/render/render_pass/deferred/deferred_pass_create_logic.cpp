
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
		auto msaaSamples = global->msaaSamples;

		auto hasMsaa = (msaaSamples != VK_SAMPLE_COUNT_1_BIT);

		auto pass = std::make_shared<Pass>();
		pass->name = Define::Pass::Deferred;
		pass->extent = {currentExtent.width, currentExtent.height};
		pass->msaaSamples = msaaSamples;

		// TODO

		// subpass0
		// 几何阶段 GeometryPass
		//	write->G-Buffer (Position+Color+Normal)

		// subpass1
		// 光照处理阶段 LightingPass
		//

		global->passMap[pass->name] = pass;
		return pass;
	}
}
