
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/light/light_comp.h"
#include "render/render_pass/render_pass_comp.h"
#include "render/render_pass/render_pass_system.h"
#include "editor/system.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    void ImGuiPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &imGuiPass = global->passes[Pass_ImGui];
        FramebufferLogic::BeginRenderPass(context, imageIndex, imGuiPass);

        Editor::RenderSystem::Update(context, imageIndex);

        FramebufferLogic::EndRenderPass(context, imageIndex);
    }
}