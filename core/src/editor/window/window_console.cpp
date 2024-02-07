
#include <imgui/imgui.h>
#include "editor/window.h"
#include "context.hpp"

namespace Editor
{
    static ImGuiTextFilter filter{};

    void Window::DrawConsole(Context *context)
    {
        if (ImGui::Begin("Console", NULL))
        {
            if (ImGui::SmallButton("Add Debug Text"))
            {
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Add Debug Error"))
            {
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Clear"))
            {
            }

            ImGui::Separator();

            ImGui::SetNextItemWidth(180);
            filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
            ImGui::Separator();

            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
            {
                if (ImGui::BeginPopupContextWindow())
                {
                    if (ImGui::Selectable("Clear"))
                    {
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::EndChild();

        }
        ImGui::End();
    }
}