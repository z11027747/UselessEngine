
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/pass/pass_logic.h"
#include "render/unit/unit_system.h"
#include "logic/camera/camera_comp.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    void GlobalMainPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &mainCameraEO = context->logicMainCameraEO;
        auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();

        if (mainCameraEO->active)
        {
            auto &mainPass = global->passes[Pass_Main];
            FramebufferLogic::BeginRenderPass(context, imageIndex, mainPass);
            UnitRenderSystem::Update(context, imageIndex, false);
            FramebufferLogic::EndRenderPass(context, imageIndex);
        }
    }
}
