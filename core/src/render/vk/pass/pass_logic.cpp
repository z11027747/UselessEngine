
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

		RenderPassLogic::AddColorAttachment(context, pass);
		RenderPassLogic::AddDepthAttachment(context, pass);
		RenderPassLogic::AddSubPass(context, pass);
		RenderPassLogic::Create(context, pass);

		CreateFramebuffers(context, pass, framebuffer);
		CreateFences(context, pass, framebuffer);
		CreateSemaphores(context, pass, framebuffer);
		AllocateCmdBuffers(context, framebuffer);

		context->renderMainPass = pass;
	}

	void PassLogic::DestroyAll(Context* context) {

		RenderPassLogic::Destroy(context, context->renderMainPass);

		DestroyFramebuffers(context, framebuffer);
		DestroyFences(context, framebuffer);
		DestroySemaphores(context, framebuffer);

	}


}
