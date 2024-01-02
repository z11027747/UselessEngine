#pragma once

#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>

class Context;

namespace Editor {

	class Global final {
	public:

		static void Create(Context*);
		static void Destroy(Context*);

		static void NewFrame(Context*);
		static void RenderDrawData(Context*, uint32_t, VkCommandBuffer&);
	};

	inline static bool ImGui_ButtonWithColor(const char* label, ImVec4 color, bool cond = true) {

		if (cond) {
			ImGui::PushStyleColor(ImGuiCol_Button, color); color.x += 0.1f;
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color); color.x += 0.1f;
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
		}

		auto result = ImGui::Button(label);

		if (cond) {
			ImGui::PopStyleColor(3);
		}

		return result;
	}

	inline static void ImGui_Drag_GlmVec3(const char* label, glm::vec3& v3,
		float width = 60, float speed = 0.02f) {

		ImGui::PushItemWidth(width);

		ImGui::DragFloat("##f0", &v3.x, speed);
		ImGui::SameLine();

		ImGui::DragFloat("##f1", &v3.y, speed);
		ImGui::SameLine();

		ImGui::DragFloat("##f2", &v3.z, speed);
		ImGui::SameLine();

		ImGui::PopItemWidth();

		ImGui::Text(label);
	}

}