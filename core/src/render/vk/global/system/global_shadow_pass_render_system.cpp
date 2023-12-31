
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/pass/pass_logic.h"
#include "render/light/light_comp.h"
#include "render/unit/unit_system.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    void GlobalShadowPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &directionLightEO = context->renderLightEOs[0];
        auto directionLight = directionLightEO->GetComponent<Render::DirectionLight>();

        if (directionLightEO->active && directionLight->hasShadow)
        {
            auto &shadowPass = global->passes[Pass_Shadow];
            FramebufferLogic::BeginRenderPass(context, imageIndex, shadowPass);
            UnitRenderSystem::Update(context, imageIndex, true);
            FramebufferLogic::EndRenderPass(context, imageIndex);
        }
    }
}
