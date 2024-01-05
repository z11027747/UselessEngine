
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/window.h"
#include "editor/global.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
    void Window::DrawViewport(Context *context, uint32_t imageIndex)
    {
        if (ImGui::Begin("Scene", NULL))
        {
            auto viewportPanelSize = ImGui::GetContentRegionAvail();
            ImGui::Image(Global::descriptors[imageIndex]->set,
                         ImVec2{viewportPanelSize.x, viewportPanelSize.y});

            ImGui::SetWindowFontScale(1.8f);
            ImGui::SetItemAllowOverlap();
            ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
            ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Viewport Size: %.fx%.f", viewportPanelSize.x, viewportPanelSize.y);
        }
        ImGui::End();

        static bool game_open = false;
        ImGui::Begin("Game", NULL);
        ImGui::End();
    }
}