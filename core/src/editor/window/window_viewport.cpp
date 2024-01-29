
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_comp.h"
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
        auto &mainPass = global->passMap[Define::Pass::Main];
        auto &clearColor = mainPass->clearColorValue.float32;

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(clearColor[0], clearColor[1], clearColor[2], 1.0f));
        if (ImGui::Begin("Scene", NULL))
        {
            ImGui::Image(ImGuiLogic::descriptor->set, ImGui::GetContentRegionAvail());

            focuesd = ImGui::IsWindowFocused();
            pos = ImGui::GetWindowPos();
            size = ImGui::GetWindowSize();

            ImGui::SetItemAllowOverlap();
            ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
            ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Viewport Size: %.fx%.f", size.x, size.y);
        }
        ImGui::End();
        ImGui::PopStyleColor();

        if (ImGui::Begin("ShadowMap", NULL))
        {
            auto &shadowPass = global->passMap[Define::Pass::Main];
            auto shadowMapExtent = shadowPass->extent;

            if (shadowMapExtent.width == shadowMapExtent.height)
            {
                auto regionMin = ImGui::GetWindowContentRegionMin();
                auto regionMax = ImGui::GetWindowContentRegionMax();
                auto x = (regionMin.x + regionMax.x) * 0.5f - shadowMapExtent.width * 0.5f;
                auto y = (regionMin.y + regionMax.y) * 0.5f - shadowMapExtent.height * 0.5f;
                ImGui::SetCursorPos(ImVec2(x, y));

                ImGui::Image(ImGuiLogic::descriptor_ShadowMap->set,
                             ImVec2(shadowMapExtent.width * 1.0f, shadowMapExtent.height * 1.0f), // size
                             ImVec2(0, 0), ImVec2(1, 1),                                          // uv0-1
                             ImVec4(1, 1, 1, 1),                                                  // tint_col
                             ImVec4(0, 0, 0, 0)                                                   // border_col
                );
            }
            else
            {
                ImGui::Image(ImGuiLogic::descriptor_ShadowMap->set, ImGui::GetContentRegionAvail());
            }
        }
        ImGui::End();
    }
}