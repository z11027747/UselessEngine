
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
#include "logic/transform/transform_comp.h"
#include "context.h"
#include "editor/global.h"

namespace Render
{

	auto enabledDebug = true;

	void System::Create(Context *context)
	{
		auto &renderGlobalEO = context->renderGlobalEO;
		renderGlobalEO = std::make_shared<EngineObject>();

		auto global = std::make_shared<Global>();
		renderGlobalEO->AddComponent<Global>(global);

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

		PassLogic::CreateMain(context);
		PipelineLogic::Create(context, "skybox", context->renderMainPass);
		PipelineLogic::Create(context, "bling_phone", context->renderMainPass);

		Editor::Global::Create(context);
	}

	void System::Update(Context *context)
	{
		CmdSubmitLogic::UpdateBatch(context);

		BufferLogic::DestroyAllTemps(context);
		CmdPoolLogic::DestroyAllTempBuffers(context);

		SwapchainLogic::WaitFence(context);
		auto imageIndex = SwapchainLogic::AcquireImageIndex(context);

		auto &vkCmdBuffer = SwapchainLogic::BeginCmd(context, imageIndex);

		auto &directionLightEO = context->renderLightEOs[0];
		auto directionLightTransform = directionLightEO->GetComponent<Logic::Transform>();
		auto &directionLightPos = directionLightTransform->position;
		auto directionLight = directionLightEO->GetComponent<Render::DirectionLight>();
		auto &directionCol = directionLight->color;
		auto &directionParams = directionLight->params;

		auto &logicCameraEOs = context->logicCameraEOs;
		for (const auto &cameraEO : logicCameraEOs)
		{
			if (!cameraEO->active)
				return;

			auto &cameraName = cameraEO->name;
			if (cameraName == G_MainCamera)
			{
				Editor::Global::NewFrame(context);
			}

			auto &renderPass = context->renderMainPass;
			FramebufferLogic::BeginRenderPass(context, imageIndex, vkCmdBuffer, renderPass);

			auto cameraTransform = cameraEO->GetComponent<Logic::Transform>();
			auto &cameraPos = cameraTransform->position;
			auto camera = cameraEO->GetComponent<Logic::Camera>();

			GlobalUBO globalUBO = {
				directionLightPos,
				camera->view,
				camera->projection,
				directionLightPos,
				directionCol,
				directionParams};

			FramebufferLogic::RenderUnits(context,
										  imageIndex, vkCmdBuffer, globalUBO);

			if (cameraName == G_MainCamera)
			{
				Editor::Global::RenderDrawData(context, imageIndex, vkCmdBuffer);
			}

			FramebufferLogic::EndRenderPass(context, imageIndex, vkCmdBuffer);
		}

		SwapchainLogic::EndAndSubmitCmd(context, imageIndex);
		SwapchainLogic::Present(context, imageIndex);
	}

	void System::Destroy(Context *context)
	{
		LogicalDeviceLogic::WaitIdle(context);

		Editor::Global::Destroy(context);

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