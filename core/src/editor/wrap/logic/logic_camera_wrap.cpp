
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include "render/vk/global/global_comp.h"
#include "editor/wrap/component_wrap.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Editor
{
	static int mode = 0;

	template <>
	void ComponentWrap<Logic::Camera>::Draw(Context *context,
											std::shared_ptr<Logic::Camera> camera, bool isInit)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &pass = global->passMap[camera->passName];

		if (isInit)
		{
			mode = static_cast<int>(camera->mode);
			return;
		}

		ImGui::Text("PassName: %s", camera->passName.data());

		if (ImGui::DragFloat("Near", &camera->near, 0.01f, 0.01f, 10.0f))
		{
			camera->far = std::max(camera->far, camera->near + 0.01f);
		}

		if (ImGui::DragFloat("Far", &camera->far, 0.02f, camera->near + 0.01f, 100.0f))
		{
		}

		if (ImGui::Combo("Mode", &mode, "Perspective\0Ortho\0"))
		{
			camera->mode = static_cast<Logic::CameraMode>(mode);
		}

		if (camera->mode == Logic::CameraMode::ePerspective)
		{
			ImGui::DragFloat("fov", &camera->fov, 0.02f, 10.0f, 90.0f);
		}
		if (camera->mode == Logic::CameraMode::eOrtho)
		{
			ImGui::DragFloat("Size", &camera->size, 0.02f);
		}

		ImGui::Spacing();
		ImGui::Text("Clear Values");

		auto &subpasses = pass->subpasses;
		for (auto &subpass : subpasses)
		{
			// color
			ImGui::ColorEdit4("ClearColor", &subpass->clearColorValue.color.float32[0]);
			// depth
			ImGui::SliderFloat2("ClearDepth", &subpass->clearDepthValue.depthStencil.depth, 0.0f, 1.0f);
			// inputs
			for (auto &clearInputValue : subpass->clearInputValues)
			{
				ImGui::ColorEdit4("Input ClearColor", &clearInputValue.color.float32[0]);
			}
		}
	}
}