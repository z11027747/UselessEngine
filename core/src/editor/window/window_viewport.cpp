
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/window.h"
#include "editor/system.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
    static bool isFocuesd;

    bool Window::IsInViewport(Context *context)
    {
        return isFocuesd;
    }

    void Window::DrawViewport(Context *context, uint32_t imageIndex)
    {
        if (ImGui::Begin("Scene", NULL))
        {
            isFocuesd = ImGui::IsWindowFocused();

            auto viewportPanelSize = ImGui::GetContentRegionAvail();
            ImGui::Image(System::descriptors[imageIndex]->set,
                         ImVec2{viewportPanelSize.x, viewportPanelSize.y});

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