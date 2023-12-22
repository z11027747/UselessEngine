
#include "render/global/global_comp.h"
#include "render/global/global_system.h"
#include "render/system_new.h"
#include "render/global/instance/instance_system.h"
#include "render/global/device/logical_device_system.h"
#include "render/global/pass/pass_system.h"
#include "render/global/framebuffer/framebuffer_system.h"
#include "render/pipeline/pipeline_system.h"
#include "render/cmd/cmd_pool_system.h"
#include "render/cmd/cmd_submit_system.h"
#include "context.h"

namespace Render {

	void System::OnCreate(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;
		renderGlobalEO = std::make_shared<EngineObject>();

		auto windowExtensions = GetWindowExtensions();
		InstanceSystem::Create(context, windowExtensions, true);
		LogicalDeviceSystem::Create(context);
		CmdPoolSystem::Create(context);

		PassSystem::Create(context);
		FramebufferSystem::Create(context);

		PipelineSystem::Create(context, "test");

	}

	void System::OnUpdate(Context* context) {

		CmdSubmitSystem::Update(context);
		CmdSubmitSystem::UpdateSemaphore(context);



	}

	void System::OnDestroy(Context* context) {

		CmdPoolSystem::Destroy(context);
		LogicalDeviceSystem::Destroy(context);
		InstanceSystem::Destroy(context);
	}


}