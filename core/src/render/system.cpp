
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/light/light_comp.h"
#include "render/unit/unit_comp.h"
#include "render/unit/unit_system.h"
#include "render/system.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "editor/system.h"
#include "context.h"

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

		DescriptorLogic::CreateGlobal(context);

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
		SwapchainLogic::BeginCmd(context, imageIndex);

		GlobalUBOUpdateSystem::Update(context);

		GlobalShadowPassRenderSystem::Update(context, imageIndex);
		GlobalMainPassRenderSystem::Update(context, imageIndex);
		GlobalImGuiPassRenderSystem::Update(context, imageIndex);

		SwapchainLogic::EndAndSubmitCmd(context, imageIndex);
		SwapchainLogic::Present(context, imageIndex);
	}

	void System::Destroy(Context *context)
	{
		LogicalDeviceLogic::WaitIdle(context);

		Editor::System::Destroy(context);

		UnitDestroySystem::Destroy(context);

		PipelineLogic::DestroyAll(context);
		PassLogic::DestroyAll(context);

		DescriptorLogic::DestroyGlobal(context);

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