
#include "render/vk/logic.h"
#include "render/vk/vk_system.hpp"
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	static bool enabledDebug = true;

	void VkCreateSystem::Create(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		globalEO = std::make_shared<EngineObject>();

		auto global = std::make_shared<Global>();
		context->AddComponent<Global>(globalEO, global);

		auto windowExtensions = GetWindowExtensions();

		InstanceLogic::Create(context, windowExtensions, true);
		InstanceLogic::CreateDebugCallback(context);

		SurfaceLogic::Create(context);
		PhysicalDeviceLogic::Find(context);
		PhysicalDeviceLogic::GetInfo(context);
		LogicalDeviceLogic::Create(context);

		CmdPoolLogic::Create(context);
		DescriptorPoolLogic::Create(context);

		SwapchainLogic::Create(context);
		SwapchainLogic::CreateImageViews(context);
		SwapchainLogic::CreateFences(context);
		SwapchainLogic::CreateSemaphores(context);
		SwapchainLogic::AllocateCmd(context);
	}
}