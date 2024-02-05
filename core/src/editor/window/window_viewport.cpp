
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_comp.h"
#include "render/post_process/post_process_comp.h"
#include "logic/camera/camera_comp.h"
#include "common/define.h"
#include "editor/window.h"
#include "editor/system.h"
#include "editor/imgui_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
    static bool focuesd;
    static ImVec2 pos;
    static ImVec2 size;

    bool Window::IsInViewport(Context *context)
    {
        return focuesd;
    }

    void Window::ToViewportNdcXY(Context *context,
                                 float &x, float &y)
    {
        auto &style = ImGui::GetStyle();

        // viewport space
        x = x - pos.x - style.WindowPadding.x;
        y = (size.y + pos.y) - y - style.WindowPadding.y;

        // ndc space
        x = 2.0f * x / size.x - 1.0f;
        y = 2.0f * y / (size.y - pos.y) - 1.0f;
    }

    void Window::DrawViewport(Context *context)
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
                ImGui::Image(ImGuiLogic::GetDescriptorSet(Descriptor_Deferred), availSize);

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

            focuesd = ImGui::IsWindowFocused();
            pos = ImGui::GetWindowPos();
            size = ImGui::GetWindowSize();

            ImGui::SetNextItemAllowOverlap();
            ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
            ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Viewport Size: %.fx%.f", size.x, size.y);
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