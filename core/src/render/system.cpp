
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
#include "render/vk/pass/pass_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/unit/unit_logic.h"
#include "render/light/light_comp.h"
#include "render/system.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "logic/transform/transform_comp.h"
#include "context.h"
#include "editor/system.h"

namespace Render
{
	auto enabledDebug = true;

	void System::Create(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		globalEO = std::make_shared<EngineObject>();

		auto global = std::make_shared<Global>();
		globalEO->AddComponent<Global>(global);

		auto windowExtensions = GetWindowExtensions();

		InstanceLogic::Create(context, windowExtensions, enabledDebug);
		if (enabledDebug)
		{
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

		/*auto imGuiPass =*/PassLogic::CreateImGui(context);
		auto mainPass = PassLogic::CreateMain(context);
		auto shadowPass = PassLogic::CreateShadow(context);

		PipelineLogic::Create(context, Pipeline_Shadow, shadowPass);
		PipelineLogic::Create(context, Pipeline_Skybox, mainPass);
		PipelineLogic::Create(context, Pipeline_Bling_Phone, mainPass);
		PipelineLogic::Create(context, Pipeline_Color, mainPass);

		Editor::System::Create(context);
	}

	void System::Update(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		CmdSubmitLogic::UpdateBatch(context);
		BufferLogic::DestroyAllTemps(context);
		CmdPoolLogic::DestroyAllTempBuffers(context);
		
		SwapchainLogic::WaitFence(context);

		auto imageIndex = SwapchainLogic::AcquireImageIndex(context);
		auto &vkCmdBuffer = SwapchainLogic::BeginCmd(context, imageIndex);

		auto &directionLightEO = context->renderLightEOs[0];
		auto directionLightTransform = directionLightEO->GetComponent<Logic::Transform>();
		auto directionLightCamera = directionLightEO->GetComponent<Logic::Camera>();
		auto directionLight = directionLightEO->GetComponent<Render::DirectionLight>();

		DirectionLightUBO directionLightUBO = {};
		if (directionLightEO->active)
		{
			directionLightUBO = {
				directionLightCamera->view,
				directionLightCamera->projection,
				-directionLightTransform->forward,
				directionLight->color,
				directionLight->params};
		}

		auto &mainCameraEO = context->logicMainCameraEO;
		auto mainCameraTransform = mainCameraEO->GetComponent<Logic::Transform>();
		auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();

		CameraUBO cameraUBO = {
			mainCameraTransform->position,
			mainCamera->view,
			mainCamera->projection,
		};

		GlobalUBO globalUBO = {
			cameraUBO,
			directionLightUBO};

		// shadow
		{
			auto &shadowPass = global->passes[Pass_Shadow];
			FramebufferLogic::BeginRenderPass(context,
											  imageIndex, vkCmdBuffer, shadowPass);
			if (directionLightEO->active && directionLight->hasShadow)
			{
				FramebufferLogic::RenderUnits(context,
											  imageIndex, vkCmdBuffer,
											  globalUBO, true);
			}
			FramebufferLogic::EndRenderPass(context, imageIndex, vkCmdBuffer);
		}

		// main
		{
			auto &mainPass = global->passes[Pass_Main];
			FramebufferLogic::BeginRenderPass(context,
											  imageIndex, vkCmdBuffer, mainPass);
			if (mainCameraEO->active)
			{
				FramebufferLogic::RenderUnits(context,
											  imageIndex, vkCmdBuffer,
											  globalUBO);
			}
			FramebufferLogic::EndRenderPass(context, imageIndex, vkCmdBuffer);
		}

		// imGui
		{
			auto &imGuiPass = global->passes[Pass_ImGui];
			FramebufferLogic::BeginRenderPass(context, imageIndex, vkCmdBuffer, imGuiPass);

			Editor::System::NewFrame(context);
			Editor::System::Draw(context, imageIndex);
			Editor::System::Render(context, vkCmdBuffer);

			FramebufferLogic::EndRenderPass(context, imageIndex, vkCmdBuffer);
		}

		SwapchainLogic::EndAndSubmitCmd(context, imageIndex);
		SwapchainLogic::Present(context, imageIndex);
	}

	void System::Destroy(Context *context)
	{
		LogicalDeviceLogic::WaitIdle(context);

		Editor::System::Destroy(context);

		UnitLogic::Destroy(context);

		PipelineLogic::DestroyAll(context);
		PassLogic::DestroyAll(context);

		SwapchainLogic::DestroyImageViews(context);
		SwapchainLogic::DestroySemaphores(context);
		SwapchainLogic::DestroyFences(context);
		SwapchainLogic::Destroy(context);

		DescriptorPoolLogic::Destroy(context);
		CmdPoolLogic::Destroy(context);

		LogicalDeviceLogic::Destroy(context);
		SurfaceLogic::Destroy(context);

		if (enabledDebug)
		{
			InstanceLogic::DestroyDebugCallback(context);
		}
		InstanceLogic::Destroy(context);
	}

}