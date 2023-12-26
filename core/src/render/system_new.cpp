
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/instance_logic.h"
#include "render/vk/global/surface_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/global/pass_logic.h"
#include "render/vk/global/framebuffer_logic.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/vk/pipeline/pipeline_system.h"
#include "render/system_new.h"
#include "context.h"

namespace Render {

	void System::OnCreate(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;
		renderGlobalEO = std::make_shared<EngineObject>();

		auto global = std::make_shared<Global>();
		renderGlobalEO->AddComponent<Global>(global);

		auto windowExtensions = GetWindowExtensions();
		auto enabledDebug = true;

		InstanceLogic::Create(context, windowExtensions, enabledDebug);
		if (enabledDebug) {
			InstanceLogic::CreateDebugCallback(context);
		}
		SurfaceLogic::Create(context);
		PhysicalDeviceLogic::Find(context);
		PhysicalDeviceLogic::GetInfo(context);
		LogicalDeviceLogic::Create(context);
		CmdPoolLogic::Create(context);
		SwapchainLogic::Create(context);
		PassLogic::Create(context);
		FramebufferLogic::Create(context);
		PipelineSystem::Create(context, "test");

	}

	void System::OnUpdate(Context* context) {
		
		CmdSubmitLogic::Update(context);
		FramebufferLogic::Update(context);

	}

	void System::OnDestroy(Context* context) {
		LogicalDeviceLogic::WaitIdle(context);

		PipelineSystem::Destroy(context);
		FramebufferLogic::Destroy(context);
		PassLogic::Destroy(context);
		SwapchainLogic::Destroy(context);
		CmdPoolLogic::Destroy(context);
		LogicalDeviceLogic::Destroy(context);
		SurfaceLogic::Destroy(context);
		InstanceLogic::Destroy(context);

	}


}