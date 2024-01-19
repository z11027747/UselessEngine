
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/component_wrap.h"
#include "logic/transform/transform_comp.h"
#include "editor/system.h"
#include "editor/test_logic.h"
#include "context.h"

namespace Editor
{
	template <>
	void ComponentWrap<Logic::Transform>::Draw(Context *context,
											   std::shared_ptr<Logic::Transform> transform, bool isFirst)
	{
		if (isFirst)
		{
			return;
		}

		ImGui_Input_GlmVec3(transform->localPosition, 0);
		ImGui::SameLine();
		if (ImGui::Button("P", ImVec2(20, 20)))
		{
			transform->localPosition = glm::vec3(0.0f);
		}
		ImGui::SameLine();
		ImGui::Text("Position");

		ImGui_Input_GlmVec3(transform->localEulerAngles, 1);
		// ImGui_Drag_GlmVec3(transform->localEulerAngles, eId, 0.05f);
		ImGui::SameLine();
		if (ImGui::Button("E", ImVec2(20, 20)))
		{
			transform->localEulerAngles = glm::vec3(0.0f);
		}
		ImGui::SameLine();
		ImGui::Text("EulerAngles");

		ImGui_Input_GlmVec3(transform->localScale, 2);
		// ImGui_Drag_GlmVec3(transform->localScale, sId, 0.02f);
		ImGui::SameLine();
		if (ImGui::Button("S", ImVec2(20, 20)))
		{
			transform->localScale = glm::vec3(1.0f);
		}
		ImGui::SameLine();
		ImGui::Text("Scale");
	}
}