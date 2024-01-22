
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
	static int modeI = 0;
	static float clearColors[4] = {0.1921569f, 0.3019608f, 0.4745098f, 0.0f};
	static float clearDepth = 1.0f;

	template <>
	void ComponentWrap<Logic::Camera>::Draw(Context *context,
											std::shared_ptr<Logic::Camera> camera, bool isInit)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &renderPass = global->passMap[camera->passName];

		if (isInit)
		{
			modeI = static_cast<int>(camera->mode);
			auto &colors = renderPass->clearColorValue.float32;
			for (auto i = 0; i < 4; i++)
			{
				clearColors[i] = colors[i];
			}
			auto &depth = renderPass->clearDepthValue.depth;
			clearDepth = depth;
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

		if (ImGui::Combo("Mode", &modeI, "Perspective\0Ortho\0"))
		{
			camera->mode = static_cast<Logic::CameraMode>(modeI);
		}

		if (camera->mode == Logic::CameraMode::ePerspective)
		{
			if (ImGui::DragFloat("fov", &camera->fov, 0.02f, 10.0f, 90.0f))
			{
			}
		}
		if (camera->mode == Logic::CameraMode::eOrtho)
		{
			if (ImGui::DragFloat("Size", &camera->size, 0.02f))
			{
			}
		}

		ImGui::Spacing();
		ImGui::Text("Clear Values");

		if (ImGui::ColorEdit4("Color", clearColors))
		{
			auto &colors = renderPass->clearColorValue.float32;
			for (auto i = 0; i < 4; i++)
			{
				colors[i] = clearColors[i];
			}
		}

		if (ImGui::InputFloat("Depth", &clearDepth))
		{
			clearDepth = std::clamp(clearDepth, 0.0f, 1.0f);

			auto &depth = renderPass->clearDepthValue.depth;
			depth = clearDepth;
		}
	}
}