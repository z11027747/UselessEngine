
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
#include "render/system.h"
#include "logic/camera/camera_comp.h"
#include "context.h"
#include "editor/global.h"

namespace Render {

	auto enabledDebug = true;

	void System::Create(Context* context) {
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
		PipelineSystem::Create(context, "skybox");
		Editor::Global::Create(context);
	}

	void System::Update(Context* context) {

		CmdSubmitLogic::Update(context);
		BufferLogic::DestroyAllTemps(context);

		Editor::Global::Update(context);

		FramebufferLogic::WaitFence(context);
		auto imageIndex = FramebufferLogic::AcquireImageIndex(context);
		FramebufferLogic::BeginRenderPass(context, imageIndex);
		FramebufferLogic::DrawUnits(context, imageIndex);
		Editor::Global::RenderData(context, imageIndex);
		FramebufferLogic::EndRenderPass(context, imageIndex);
		FramebufferLogic::Present(context, imageIndex);
	}

	void System::Destroy(Context* context) {

		LogicalDeviceLogic::WaitIdle(context);

		Editor::Global::Destroy(context);
		PipelineSystem::DestroyAll(context);
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