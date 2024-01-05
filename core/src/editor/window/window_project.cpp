
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/window.h"
#include "editor/global.h"
#include "context.h"
#include "engine_object.h"

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