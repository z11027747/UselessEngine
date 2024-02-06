
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/light/light_comp.h"
#include "render/render_pass/render_pass_logic.h"
#include "render/render_pass/render_pass_system.h"
#include "render/mesh/mesh_logic.h"
#include "render/mesh/mesh_system.h"
#include "render/material/material_logic.h"
#include "render/material/material_system.h"
#include "render/system.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "editor/system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	auto enabledDebug = true;

	void System::Create(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		globalEO = std::make_shared<EngineObject>();

		auto global = std::make_shared<Global>();
		context->AddComponent<Global>(globalEO, global);

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

		MaterialGlobalDescriptorLogic::Create(context);

		RenderPassLogic::CreateImGui(context);
		auto shadowPass = RenderPassLogic::CreateShadow(context);
		auto forwardPass = RenderPassLogic::CreateForward(context);
		auto deferredPass = RenderPassLogic::CreateDeferred(context);
		auto postProcessPass = RenderPassLogic::CreatePostProcess(context);

		PipelineLogic::Create(context, Define::Pipeline::Shadow, shadowPass);
		PipelineLogic::Create(context, Define::Pipeline::Skybox, forwardPass);
		PipelineLogic::Create(context, Define::Pipeline::LightModel, forwardPass);
		PipelineLogic::Create(context, Define::Pipeline::Color, forwardPass);
		PipelineLogic::Create(context, Define::Pipeline::Deferred_LightModel, deferredPass);
		PipelineLogic::Create(context, Define::Pipeline::Deferred_Shading, deferredPass);
		PipelineLogic::Create(context, Define::Pipeline::Deferred_Volumn, deferredPass);
		PipelineLogic::Create(context, Define::Pipeline::Deferred_PointLight, deferredPass);
		PipelineLogic::Create(context, Define::Pipeline::PostProcess_Bloom, postProcessPass);
		PipelineLogic::Create(context, Define::Pipeline::PostProcess_ToonMapping, postProcessPass);
		PipelineLogic::Create(context, Define::Pipeline::PostProcess_Global, postProcessPass);

		RenderPassLogic::CreateDeferredDescriptor(context);
		RenderPassLogic::CreatePostProcessDescriptor(context);

		MeshInstanceLogic::CreateCache(context);
		MaterialInstanceLogic::CreateCache(context);

		Editor::System::Create(context);
	}

	void System::Update(Context *context)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Global>();

		{
			MeshInstanceCreateSystem::Update(context);
			MaterialInstanceCreateSystem::Update(context);
		}

		CmdSubmitLogic::BatchAll(context);
		BufferLogic::DestroyAllTemps(context);
		CmdPoolLogic::DestroyAllTempBuffers(context);

		SwapchainLogic::WaitFence(context);

		auto imageIndex = SwapchainLogic::AcquireImageIndex(context);
		SwapchainLogic::BeginCmd(context, imageIndex);

		{
			MaterialGlobalUBOUpdateSystem::Update(context);
			MaterialUBOUpdateSystem::Update(context);

			ShadowPassRenderSystem::Update(context, imageIndex);
			ForwardPassRenderSystem::Update(context, imageIndex);
			DeferredPassRenderSystem::Update(context, imageIndex);
			PostProcessPassRenderSystem::Update(context, imageIndex);
			ImGuiPassRenderSystem::Update(context, imageIndex);
		}

		SwapchainLogic::EndAndSubmitCmd(context, imageIndex);
		SwapchainLogic::Present(context, imageIndex);
	}

	void System::Destroy(Context *context)
	{
		LogicalDeviceLogic::WaitIdle(context);

		Editor::System::Destroy(context);

		{
			MeshDestroySystem::Destroy(context);
			MeshInstanceLogic::DestroyCache(context);
			MaterialDestroySystem::Destroy(context);
			MaterialInstanceLogic::DestroyCache(context);
		}

		PipelineLogic::DestroyAll(context);

		RenderPassLogic::DestroyDeferredDescriptor(context);
		RenderPassLogic::DestroyAll(context);

		MaterialGlobalDescriptorLogic::Destroy(context);

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