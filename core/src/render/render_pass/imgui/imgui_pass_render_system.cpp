
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_system.h"
#include "render/render_pass/render_pass_logic.h"
#include "editor/imgui/imgui_system.hpp"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

class Context;

namespace Render
{
    void ImGuiPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &imGuiPass = global->passMap[Define::Pass::ImGui];
        FramebufferLogic::BeginRenderPass(context, imageIndex, imGuiPass);
        {
            Editor::ImGuiRenderPassLogic::Draw(context, imageIndex);
        }
        FramebufferLogic::EndRenderPass(context, imageIndex);
    }
}