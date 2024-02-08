
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_vulkan.h"
#include "editor/imgui/imgui_system.hpp"
#include "editor/imgui/imgui_logic.h"
#include "editor/window/window_logic.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    void ImGuiDestroySystem::Destroy(Context *context)
    {
        ImGuiLogic::DestroyDescriptorSetLayout(context);
        ImGuiLogic::DestroyImGui(context);
    }
}