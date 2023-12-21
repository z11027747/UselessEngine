
#include "render/global/global_def.h"
#include "render/system_new.h"
#include "render/instance/instance_system.h"
#include "render/device/logical_device_system.h"
#include "render/cmd/cmd_pool_system.h"
#include "context.h"

namespace Render {

	void System::OnCreate(Context* context) {

		CreateEO(context);

		auto windowExtensions = GetWindowExtensions();
		InstanceSystem::Create(context, windowExtensions, true);
		LogicalDeviceSystem::Create(context);
		CmdPoolSystem::Create(context);
	}

	void System::OnUpdate(Context* context) {

	}

	void System::OnDestroy(Context* context) {

		CmdPoolSystem::Destroy(context);
		LogicalDeviceSystem::Destroy(context);
		InstanceSystem::Destroy(context);
	}


	void System::CreateEO(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;
		renderGlobalEO = std::make_shared<EngineObject>();
	}

	//获取和glfw窗口系统交互的扩展
	std::vector<const char*> System::GetWindowExtensions()
	{
		uint32_t glfwExtensionCount;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> windowExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		return windowExtensions;
	}

}