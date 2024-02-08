
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_comp.h"
#include "render/post_process/post_process_comp.h"
#include "logic/camera/camera_comp.h"
#include "editor/window/window_system.hpp"
#include "editor/window/window_logic.hpp"
#include "editor/imgui/imgui_logic.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    void WindowViewportSystem::Update(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Render::Global>();

        if (ImGui::Begin("Scene", NULL))
        {
            auto availSize = ImGui::GetContentRegionAvail();

            auto &mainCameraEO = context->logicMainCameraEO;
            auto camera = mainCameraEO->GetComponent<Logic::Camera>();
            auto postProcess = mainCameraEO->GetComponent<Render::PostProcess>();

            auto &passName = camera->passName;
            if (passName == Define::Pass::Forward)
            {
                if (postProcess != nullptr)
                {
                    ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_PostProcess), availSize);
                }
                else
                {
                    ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_Forward), availSize);
                }
            }
            else if (passName == Define::Pass::Deferred)
            {
                if (postProcess != nullptr)
                {
                    ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_PostProcess), availSize);
                }
                else
                {
                    ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_Deferred), availSize);
                }

                // ImGui::SetItemAllowOverlap();
                auto size = ImVec2(120.0f, 120.0f);
                auto fontHeight = 20.0f;
                auto min = ImGui::GetWindowContentRegionMin();
                auto max = ImGui::GetWindowContentRegionMax();
                auto pos = ImVec2(min.x, max.y - size.y - fontHeight);
                ImGui::SetCursorPos(pos);
                ImGui::Text("gBuffer (position, normal, color, material)");
                ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_Deferred_Position), size,
                             ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
                ImGui::SameLine();
                ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_Deferred_Normal), size,
                             ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
                ImGui::SameLine();
                ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_Deferred_Color), size,
                             ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
                ImGui::SameLine();
                ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_Deferred_Material), size,
                             ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
            }

            WindowLogic::viewportFocuesd = ImGui::IsWindowFocused();
            WindowLogic::viewportPos = ImGui::GetWindowPos();
            WindowLogic::viewportSize = ImGui::GetWindowSize();

            ImGui::SetNextItemAllowOverlap();
            ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
            ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Viewport Size: %.fx%.f", WindowLogic::viewportSize.x, WindowLogic::viewportSize.y);
        }
        ImGui::End();

        if (ImGui::Begin("ShadowMap", NULL))
        {
            ImGui::Image(ImGuiLogic::GetDescriptorSet(Define::Pass::Shadow),
                         ImGui::GetContentRegionAvail(), // size
                         ImVec2(0, 0), ImVec2(1, 1),     // uv0-1
                         ImVec4(1, 1, 1, 1),             // tint_col
                         ImVec4(0, 0, 0, 0)              // border_col
            );
        }
        ImGui::End();
    }
}