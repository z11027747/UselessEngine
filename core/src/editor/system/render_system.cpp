
#include <vulkan/vulkan.h>
#include "editor/imgui_demo/imgui_impl_glfw.h"
#include "editor/imgui_demo/imgui_impl_vulkan.h"
#include "render/vk/global/global_comp.h"
#include "editor/imgui_logic.h"
#include "editor/system.h"
#include "editor/window.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    void RenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool showDemoWindow = true;
        if (showDemoWindow)
        {
            ImGui::ShowDemoWindow(&showDemoWindow);
            // return;
        }

        const auto *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoBackground;

        bool p_open = true;
        if (ImGui::Begin("##DockSpace-FullScreen", &p_open, window_flags))
        {
            auto &io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                auto dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
            }

            Window::Draw(context);
        }
        ImGui::End();

        ImGui::PopStyleVar(2);

        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Render::Global>();
        auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

        ImGui::Render();
        auto *main_draw_data = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(main_draw_data, vkCmdBuffer);
    }

}