#pragma once

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <string>

class Context;

namespace Editor
{
    class TestLogic final
    {
    public:
        static void CreateImGui(Context *);
        static void DestroyImGui(Context *);

        static void CreateDescriptor(Context *);
        static void DestroyDescriptor(Context *);

        static void CreateMainCamera(Context *);
        static void CreateLight(Context *);
        static void CreateSkybox(Context *);
        static void CreateCubes(Context *);
        static void CreateCube(Context *, const glm::vec3 &, float, const std::string &);
        static void CreateAxis(Context *);
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

    inline static void ImGui_Drag_GlmVec3(glm::vec3 &v3, int id, float speed, float width = 60)
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

    inline static void ImGui_Input_GlmVec3(glm::vec3 &v3, int id, float width = 60)
    {
        ImGui::PushID(id);
        ImGui::PushItemWidth(width);

        ImGui::InputFloat("##f0", &v3.x);
        ImGui::SameLine();

        ImGui::InputFloat("##f1", &v3.y);
        ImGui::SameLine();

        ImGui::InputFloat("##f2", &v3.z);
        ImGui::SameLine();

        ImGui::PopItemWidth();
        ImGui::PopID();
    }
}