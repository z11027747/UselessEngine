
#include <imgui/imgui.h>
#include "editor/window.h"
#include "context.h"

namespace Editor
{
    void Window::DrawProject(Context *context)
    {
        if (ImGui::Begin("Project", NULL))
        {
        }
        ImGui::End();
    }
}