
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_comp.h"
#include "render/post_process/post_process_comp.h"
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
            auto &mainCameraEO = context->logicMainCameraEO;
            auto postProcess = mainCameraEO->GetComponent<Render::PostProcess>();
            if (postProcess != nullptr)
            {
                ImGui::Image(ImGuiLogic::GetDescriptorSet(Define::Pass::PostProcess), ImGui::GetContentRegionAvail());
            }
            else
            {
                ImGui::Image(ImGuiLogic::GetDescriptorSet(Define::Pass::Main), ImGui::GetContentRegionAvail());
            }

            focuesd = ImGui::IsWindowFocused();
            pos = ImGui::GetWindowPos();
            size = ImGui::GetWindowSize();

            ImGui::SetItemAllowOverlap();
            ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
            ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Viewport Size: %.fx%.f", size.x, size.y);
        }
        ImGui::End();

        if (ImGui::Begin("ShadowMap", NULL))
        {
            auto &shadowPass = global->passMap[Define::Pass::Shadow];
            auto shadowMapExtent = shadowPass->extent;

            auto regionMin = ImGui::GetWindowContentRegionMin();
            auto regionMax = ImGui::GetWindowContentRegionMax();
            auto x = (regionMin.x + regionMax.x) * 0.5f - shadowMapExtent.width * 0.5f;
            auto y = (regionMin.y + regionMax.y) * 0.5f - shadowMapExtent.height * 0.5f;
            ImGui::SetCursorPos(ImVec2(x, y));

            ImGui::Image(ImGuiLogic::GetDescriptorSet(Define::Pass::Shadow),
                         ImVec2(shadowMapExtent.width * 1.0f, shadowMapExtent.height * 1.0f), // size
                         ImVec2(0, 0), ImVec2(1, 1),                                          // uv0-1
                         ImVec4(1, 1, 1, 1),                                                  // tint_col
                         ImVec4(0, 0, 0, 0)                                                   // border_col
            );
        }
        ImGui::End();
    }
}