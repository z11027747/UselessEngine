
#include "render/vk/global/global_system.h"
#include "render/vk/vk_system.hpp"
#include "render/light/light_system.h"
#include "render/render_pass/render_pass_system.h"
#include "render/mesh/mesh_system.h"
#include "render/material/material_system.h"
#include "render/post_process/post_process_system.h"
#include "render/system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
	void System::Create(Context *context)
	{
		VkCreateSystem::Create(context);
		RenderPassCreateSystem::Create(context);
		MaterialCreateSystem::Create(context);
		MeshCreateSystem::Create(context);
		LightCreateSystem::Create(context);
		MeshCreateSystem::Create(context);
		PostProcessCreateSystem::Create(context);
	}
	void System::Update(Context *context)
	{
		MeshInstanceUpdateSystem::Update(context);
		MaterialInstanceUpdateSystem::Update(context);
		MaterialGlobalUBOUpdateSystem::Update(context);
		MaterialUBOUpdateSystem::Update(context);

		VkBeginRenderSystem::Update(context);
		{
			ShadowPassRenderSystem::Update(context);
			ForwardPassRenderSystem::Update(context);
			DeferredPassRenderSystem::Update(context);
			PostProcessPassRenderSystem::Update(context);
			ImGuiPassRenderSystem::Update(context);
		}
		VkEndRenderSystem::Update(context);
	}
	void System::Destroy(Context *context)
	{
		MeshDestroySystem::Destroy(context);
		MaterialDestroySystem::Destroy(context);
		RenderPassDestroySystem::Destroy(context);
		VkDestroySystem::Destroy(context);
	}
}