
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/instance_logic.h"
#include "render/vk/global/surface_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/global/descriptor_pool_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_system.h"
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pass/pass_logic.h"
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
		DescriptorPoolLogic::Create(context);

		SwapchainLogic::Create(context);
		SwapchainLogic::CreateImageViews(context);
		SwapchainLogic::CreateFences(context);
		SwapchainLogic::CreateSemaphores(context);
		SwapchainLogic::AllocateCmd(context);

		PassLogic::CreateMain(context);
		PipelineLogic::Create(context, "test", context->renderMainPass);
		PipelineLogic::Create(context, "skybox", context->renderMainPass);

		Editor::Global::Create(context);
	}

	void System::Update(Context* context) {

		CmdSubmitLogic::UpdateBatch(context);

		BufferLogic::DestroyAllTemps(context);
		CmdPoolLogic::DestroyAllTemps(context);

		SwapchainLogic::WaitFence(context);
		auto imageIndex = SwapchainLogic::AcquireImageIndex(context);

		auto& vkCmdBuffer = SwapchainLogic::BeginCmd(context, imageIndex);

		auto& logicCameraEOs = context->logicCameraEOs;
		for (const auto& cameraEO : logicCameraEOs) {
			auto camera = cameraEO->GetComponent<Logic::Camera>();
			if (!cameraEO->active)
				return;

			auto& cameraName = cameraEO->name;
			if (cameraName == G_MainCamera) {
				Editor::Global::NewFrame(context);
			}

			GlobalUBO globalUBO = {
				camera->view,
				camera->projection
			};

			auto& renderPass = context->renderMainPass;
			FramebufferLogic::BeginRenderPass(context, imageIndex, vkCmdBuffer, renderPass);

			FramebufferLogic::RenderUnits(context,
				imageIndex, vkCmdBuffer, globalUBO);

			if (cameraName == G_MainCamera) {
				Editor::Global::RenderDrawData(context, imageIndex, vkCmdBuffer);
			}

			FramebufferLogic::EndRenderPass(context, imageIndex, vkCmdBuffer);
		}

		SwapchainLogic::EndAndSubmitCmd(context, imageIndex);
		SwapchainLogic::Present(context, imageIndex);
	}

	void System::Destroy(Context* context) {

		LogicalDeviceLogic::WaitIdle(context);

		Editor::Global::Destroy(context);
		//PipelineSystem::DestroyAll(context);
		DescriptorPoolLogic::Destroy(context);
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