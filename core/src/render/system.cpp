
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/instance_logic.h"
#include "render/vk/global/surface_logic.h"
#include "render/vk/global/physical_device_logic.h"
#include "render/vk/global/logical_device_logic.h"
#include "render/vk/global/swapchain_logic.h"
#include "render/vk/global/descriptor_pool_logic.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/pipeline_system.h"
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/framebuffer/framebuffer_comp.h"
#include "render/vk/framebuffer/framebuffer_logic.h"
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
		DescriptorPoolLogic::Create(context);

		//auto& mainCameraEO = context->GetEO(G_MainCamera);
		//auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();
		//PipelineLogic::Create(context, "test", mainCamera->renderPass);
		//PipelineLogic::Create(context, "skybox", mainCamera->renderPass);

		Editor::Global::Create(context);
	}

	void System::Update(Context* context) {

		BufferLogic::DestroyAllTemps(context);
		CmdPoolLogic::FreeAllTemps(context);

		CmdSubmitLogic::UpdateBatch(context);

		SwapchainLogic::WaitFence(context);
		auto imageIndex = SwapchainLogic::AcquireImageIndex(context);

		auto tempCmd = CmdPoolLogic::CreateTempCmd(context);

		auto& logicCameraEOs = context->logicCameraEOs;
		for (const auto& cameraEO : logicCameraEOs) {
			auto camera = cameraEO->GetComponent<Logic::Camera>();
			if (!cameraEO->active)
				return;

			if (cameraEO->name == G_MainCamera) {
				Editor::Global::NewFrame(context);
			}

			GlobalUBO globalUBO = {
				camera->view,
				camera->projection
			};

			CmdPoolLogic::Allocate(context, tempCmd, 1);
			auto& vkCmdBuffer = tempCmd->vkCmdBuffers.back();

			FramebufferLogic::BeginCmd(context, imageIndex, vkCmdBuffer);
			{
				auto& renderPass = camera->renderPass;
				FramebufferLogic::BeginRenderPass(context,
					framebuffer, renderPass, imageIndex);
				{
					FramebufferLogic::RenderUnits(context,
						framebuffer, imageIndex,
						globalUBO);

					if (cameraEO->name == G_MainCamera) {
						Editor::Global::RenderDrawData(context,
							framebuffer, imageIndex);
					}
				}
				FramebufferLogic::EndRenderPass(context,
					framebuffer, renderPass, imageIndex);
			}

			FramebufferLogic::EndCmd(context, imageIndex, vkCmdBuffer);
		}

		SwapchainLogic::Submit(context, imageIndex, tempCmd->vkCmdBuffers);
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