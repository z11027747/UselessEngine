
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/component_wrap.h"
#include "logic/transform/transform_comp.h"
#include "editor/system.h"
#include "editor/imgui_logic.h"
#include "context.hpp"

namespace Editor
{
	static bool dragMode = false;

	template <>
	void ComponentWrap<Logic::Transform>::Draw(Context *context,
											   std::shared_ptr<Logic::Transform> transform, bool isInit)
	{
		if (isInit)
		{
			return;
		}

		ImGui::Checkbox("UseDrag", &dragMode);

		if (dragMode)
			ImGui_Drag_GlmVec3(transform->localPosition, 0, 0.02f);
		else
			ImGui_Input_GlmVec3(transform->localPosition, 0);
		ImGui::SameLine();
		if (ImGui::Button("P", ImVec2(20, 20)))
		{
			transform->localPosition = glm::vec3(0.0f);
		}
		ImGui::SameLine();
		ImGui::Text("Position");

		if (dragMode)
			ImGui_Drag_GlmVec3(transform->localEulerAngles, 1, 0.05f);
		else
			ImGui_Input_GlmVec3(transform->localEulerAngles, 1);
		ImGui::SameLine();
		if (ImGui::Button("E", ImVec2(20, 20)))
		{
			transform->localEulerAngles = glm::vec3(0.0f);
		}
		ImGui::SameLine();
		ImGui::Text("EulerAngles");

		if (dragMode)
			ImGui_Drag_GlmVec3(transform->localScale, 2, 0.02f);
		else
			ImGui_Input_GlmVec3(transform->localScale, 2);
		ImGui::SameLine();
		if (ImGui::Button("S", ImVec2(20, 20)))
		{
			transform->localScale = glm::vec3(1.0f);
		}
		ImGui::SameLine();
		ImGui::Text("Scale");
	}
}