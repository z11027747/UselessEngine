
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/system_new.h"
#include "render/global/instance_system.h"
#include "render/global/logical_device_system.h"
#include "render/global/physical_device_system.h"
#include "render/global/pass_system.h"
#include "render/global/swapchain_system.h"
#include "render/global/framebuffer_system.h"
#include "render/global/surface_system.h"
#include "render/pipeline/pipeline_system.h"
#include "render/cmd/cmd_pool_system.h"
#include "render/cmd/cmd_submit_system.h"
#include "context.h"

namespace Render {

	void System::OnCreate(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;
		renderGlobalEO = std::make_shared<EngineObject>();

		auto global = std::make_shared<Global>();
		renderGlobalEO->AddComponent<Global>(global);

		auto windowExtensions = GetWindowExtensions();
		InstanceSystem::Create(context, windowExtensions, true);
		SurfaceSystem::Create(context);
		PhysicalDeviceSystem::Create(context);
		LogicalDeviceSystem::Create(context);
		CmdPoolSystem::Create(context);
		SwapchainSystem::Create(context);
		PassSystem::Create(context);
		FramebufferSystem::Create(context);
		PipelineSystem::Create(context, "test");

	}

	void System::OnUpdate(Context* context) {

		CmdSubmitSystem::Update(context);
		FramebufferSystem::Update(context);

	}

	void System::OnDestroy(Context* context) {
		LogicalDeviceSystem::WaitIdle(context);

		PipelineSystem::DestroyAll(context);
		FramebufferSystem::Destroy(context);
		PassSystem::Destroy(context);
		SwapchainSystem::Destroy(context);
		CmdPoolSystem::Destroy(context);
		LogicalDeviceSystem::Destroy(context);
		SurfaceSystem::Destroy(context);
		InstanceSystem::Destroy(context);

	}


}