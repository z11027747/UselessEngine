
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/window.h"
#include "editor/system.h"
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
        if (ImGui::Begin("Scene", NULL))
        {
            focuesd = ImGui::IsWindowFocused();
            pos = ImGui::GetWindowPos();
            size = ImGui::GetWindowSize();

            ImGui::Image(System::descriptor->set, ImGui::GetContentRegionAvail());

            ImGui::SetItemAllowOverlap();
            ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
            ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "Viewport Size: %.fx%.f", size.x, size.y);
        }
        ImGui::End();

        static bool game_open = false;
        ImGui::Begin("Game", NULL);
        ImGui::End();
    }
}