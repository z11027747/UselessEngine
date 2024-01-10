
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/unit/unit_system.h"
#include "render/light/light_comp.h"
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

        auto &directionLightEO = context->renderLightEOs[0];
        auto directionLight = directionLightEO->GetComponent<Render::DirectionLight>();

        auto &imGuiPass = global->passes[Pass_ImGui];
        FramebufferLogic::BeginRenderPass(context, imageIndex, imGuiPass);
        Editor::RenderSystem::Update(context, imageIndex);
        FramebufferLogic::EndRenderPass(context, imageIndex);
    }
}