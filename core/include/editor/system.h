#pragma once

#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "render/vk/pipeline/pipeline_comp.h"

class Context;

namespace Editor
{
	class System final
	{
	public:
		static void Create(Context *);
		static void Destroy(Context *);

		static VkDescriptorSetLayout descriptorSetLayout;
		static std::vector<std::shared_ptr<Render::Descriptor>> descriptors;
		static void CreateDescriptors(Context *);
		static void DestroyDescriptors(Context *);

		static void NewFrame(Context *);
		static void Draw(Context *, uint32_t);
		static void Render(Context *, VkCommandBuffer &);
	};

	inline static bool ImGui_ButtonWithColor(const char *label, ImVec4 color, bool cond = true)
	{
		if (cond)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, color);
			color.x += 0.1f;
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
			color.x += 0.1f;
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
		}

		auto result = ImGui::Button(label);

		if (cond)
		{
			ImGui::PopStyleColor(3);
		}

		return result;
	}

	inline static void ImGui_Drag_GlmVec3(glm::vec3 &v3, int id, float speed,   float width = 60)
	{
		ImGui::PushID(id);
		ImGui::PushItemWidth(width);

		ImGui::DragFloat("##f0", &v3.x, speed);
		ImGui::SameLine();

		ImGui::DragFloat("##f1", &v3.y, speed);
		ImGui::SameLine();

		ImGui::DragFloat("##f2", &v3.z, speed);
		ImGui::SameLine();

		ImGui::PopItemWidth();
		ImGui::PopID();
	}

}