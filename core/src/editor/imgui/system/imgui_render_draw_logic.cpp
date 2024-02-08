
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_vulkan.h"
#include "editor/imgui/imgui_system.hpp"
#include "editor/window/window_logic.hpp"
#include "editor/window/window_system.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    void ImGuiRenderPassLogic::Draw(Context *context, uint32_t imageIndex)
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

        ImGui::Begin("##DockSpace-FullScreen", NULL, window_flags);

        auto &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            auto dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }

        WindowMenuBarSystem::Update(context);
        WindowHierachySystem::Update(context);
        WindowInspectorSystem::Update(context);
        WindowViewportSystem::Update(context);
        WindowConsoleSystem::Update(context);
        WindowProjectSystem::Update(context);

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