
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include <algorithm>
#include "logic/camera/camera_logic.h"
#include "editor/wrap/logic_component_wrap.h"
#include "editor/global.h"
#include "context.h"

namespace Editor
{
	void LogicComponentWrap::DrawCamera(Context *context,
										std::shared_ptr<Logic::Camera> camera)
	{
		auto &frustum = camera->frustum;

		if (ImGui::DragFloat("Near", &frustum.near, 0.01f, 0.01f, 10.0f))
		{
			frustum.far = std::max(frustum.far, frustum.near + 0.01f);

			Logic::CameraLogic::UpdateProjection(camera);
		}

		if (ImGui::DragFloat("Far", &frustum.far, 0.02f, frustum.near + 0.01f, 100.0f))
		{
			Logic::CameraLogic::UpdateProjection(camera);
		}

		if (ImGui::DragFloat("FOV", &frustum.fov, 0.02f, 10.0f, 90.0f))
		{
			Logic::CameraLogic::UpdateProjection(camera);
		}

		ImGui::Text("Clear Values");

		static float clearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
		static float clearDepth = 1.0f;

		if (ImGui::ColorEdit4("Color", clearColor))
		{
			auto &colors = camera->renderPass->clearColorValue.float32;
			colors[0] = clearColor[0];
			colors[1] = clearColor[1];
			colors[2] = clearColor[2];
			colors[3] = clearColor[3];
		}

		if (ImGui::InputFloat("Depth", &clearDepth))
		{
			clearDepth = std::clamp(clearDepth, 0.0f, 1.0f);

			auto &depth = camera->renderPass->clearDepthValue.depth;
			depth = clearDepth;
		}
	}
}