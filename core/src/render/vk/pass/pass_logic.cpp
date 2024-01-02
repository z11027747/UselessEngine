
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "context.h"

namespace Render {

	void PassLogic::CreateMain(Context* context) {

		auto pass = std::make_shared<Pass>();

		RenderPassLogic::CreateColorAttachment(context, pass);
		RenderPassLogic::CreateDepthAttachment(context, pass);

		RenderPassLogic::CreatColorImage2dsBySwapchain(context, pass);
		RenderPassLogic::CreateDepthImage2ds(context, pass);

		RenderPassLogic::AddSubPass(context, pass);
		RenderPassLogic::Create(context, pass);

		FramebufferLogic::Create(context, pass);

		context->renderMainPass = pass;
	}

	void PassLogic::DestroyAll(Context* context) {


	}


}
