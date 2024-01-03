
#include <imgui/imgui.h>
#include <memory>
#include <iostream>
#include "editor/wrap/engine_object_wrap.h"
#include "editor/window.h"
#include "editor/global.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
    void Window::DrawViewport(Context *context, uint32_t imageIndex)
    {
        auto sceneVisible = ImGui::Begin("Scene", NULL);
        if (sceneVisible)
        {
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            ImGui::Image(Global::descriptors[imageIndex]->set,
                         ImVec2{viewportPanelSize.x, viewportPanelSize.y});
        }
        ImGui::End();

        static bool game_open = false;
        ImGui::Begin("Game", NULL);
        ImGui::End();
    }
}