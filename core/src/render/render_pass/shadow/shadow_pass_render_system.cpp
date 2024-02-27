
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/light/light_comp.h"
#include "render/render_pass/render_pass_system.h"
#include "render/render_pass/render_pass_logic.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

class Context;

namespace Render
{
    void ShadowPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &shadowPass = global->passMap[Define::Pass::Shadow];
        FramebufferLogic::BeginRenderPass(context, imageIndex, shadowPass);

        if (context->renderLightEOs.size() > 0)
        {
            auto &directionLightEO = context->renderLightEOs[0];
            if (directionLightEO->HasComponent<Render::DirectionLight>())
            {
                auto directionLight = directionLightEO->GetComponent<Render::DirectionLight>();
                if (directionLightEO->active && directionLight->hasShadow)
                {
                    RenderPassLogic::Draw(context, true);
                }
            }
        }

        FramebufferLogic::EndRenderPass(context);
    }
}