
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "logic/camera/camera_logic.h"
#include "editor/wrap/logic_component_wrap.h"
#include "editor/global.h"
#include "context.h"

namespace Editor {

	void LogicComponentWrap::DrawCamera(Context* context,
		std::shared_ptr<Logic::Camera> camera
	) {
		auto& frustum = camera->frustum;

		if (ImGui::DragFloat("Near", &frustum.near, 0.01f, 0.01f, 10.0f)) {
			if (frustum.far <= frustum.near) {
				frustum.far = frustum.near + 0.01f;
			}

			Logic::CameraLogic::UpdateProjection(camera);
		}

		if (ImGui::DragFloat("Far", &frustum.far, 0.02f, frustum.near + 0.01f, 100.0f)) {
			Logic::CameraLogic::UpdateProjection(camera);
		}

		if (ImGui::DragFloat("FOV", &frustum.fov, 0.02f, 10.0f, 90.0f)) {
			Logic::CameraLogic::UpdateProjection(camera);
		}

	}

}