
#include "render/global/global_def.h"
#include "render/system_new.h"
#include "render/instance/instance_system.h"
#include "render/device/logical_device_system.h"
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