
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/logic_component_wrap.h"
#include "editor/global.h"
#include "context.h"

namespace Editor
{
	void LogicComponentWrap::DrawTransform(Context *context,
										   std::shared_ptr<Logic::Transform> transform)
	{
		static int pId = 0;
		ImGui_Drag_GlmVec3(transform->position, pId, 0.02f);
		ImGui::SameLine();
		if (ImGui::Button("P", ImVec2(20, 20)))
		{
			transform->position = glm::vec3(0.0f);
		}
		ImGui::SameLine();
		ImGui::Text("Position");

		static int eId = 1;
		ImGui_Drag_GlmVec3(transform->eulerAngles, eId, 0.05f);
		ImGui::SameLine();
		if (ImGui::Button("E", ImVec2(20, 20)))
		{
			transform->eulerAngles = glm::vec3(0.0f);
		}
		ImGui::SameLine();
		ImGui::Text("EulerAngles");

		static int sId = 2;
		ImGui_Drag_GlmVec3(transform->scale, sId, 0.02f);
		ImGui::SameLine();
		if (ImGui::Button("S", ImVec2(20, 20)))
		{
			transform->scale = glm::vec3(1.0f);
		}
		ImGui::SameLine();
		ImGui::Text("Scale");
	}
}