
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/instance_logic.h"
#include "render/vk/global/surface_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/global/pass_logic.h"
#include "render/vk/global/framebuffer_logic.h"
#include "render/vk/global/descriptor_pool_logic.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_system.h"
#include "render/unit/unit_logic.h"
#include "render/system_new.h"
#include "context.h"
#include "editor/window.h"

namespace Render {

	auto enabledDebug = true;

	void System::OnCreate(Context* context) {
		auto& renderGlobalEO = context->renderGlobalEO;
		renderGlobalEO = std::make_shared<EngineObject>();

		auto global = std::make_shared<Global>();
		renderGlobalEO->AddComponent<Global>(global);

		auto windowExtensions = GetWindowExtensions();

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
		DescriptorPoolLogic::Create(context);
		PipelineSystem::Create(context, "test");
		Editor::Window::Create(context);
	}

	void System::OnUpdate(Context* context) {

		Editor::Window::Update(context);

		CmdSubmitLogic::Update(context);
		BufferLogic::DestroyAllTemps(context);

		FramebufferLogic::WaitFence(context);
		auto imageIndex = FramebufferLogic::AcquireImageIndex(context);
		FramebufferLogic::BeginRenderPass(context, imageIndex);
		FramebufferLogic::DrawUnits(context, imageIndex);
		Editor::Window::DrawData(context, imageIndex);
		FramebufferLogic::EndRenderPass(context, imageIndex);
		FramebufferLogic::Present(context, imageIndex);
	}

	void System::OnDestroy(Context* context) {

		LogicalDeviceLogic::WaitIdle(context);

		Editor::Window::Destroy(context);
		PipelineSystem::Destroy(context);
		DescriptorPoolLogic::Destroy(context);
		FramebufferLogic::Destroy(context);
		PassLogic::Destroy(context);
		SwapchainLogic::Destroy(context);
		CmdPoolLogic::Destroy(context);
		UnitLogic::Destroy(context);
		LogicalDeviceLogic::Destroy(context);
		SurfaceLogic::Destroy(context);
		if (enabledDebug) {
			InstanceLogic::DestroyDebugCallback(context);
		}
		InstanceLogic::Destroy(context);
	}


}