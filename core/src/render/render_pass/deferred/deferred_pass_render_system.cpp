
#include "render/vk/global/global_comp.h"
#include "render/vk/pass/pass_logic.h"
#include "render/render_pass/render_pass_system.h"
#include "logic/camera/camera_comp.h"
#include "common/define.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    void DeferredPassRenderSystem::Update(Context *context, uint32_t imageIndex)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &mainCameraEO = context->logicMainCameraEO;
        if (mainCameraEO == nullptr)
            return;

        auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();
        if (mainCameraEO->active && mainCamera->passName == Define::Pass::Deferred)
        {
            auto &deferredPass = global->passMap[Define::Pass::Deferred];
            FramebufferLogic::BeginRenderPass(context, imageIndex, deferredPass);

            // subpass0: geometryPass
            RenderPassSystem::Update(context, imageIndex, deferredPass);

            FramebufferLogic::NextSubpass(context, imageIndex);

            // subpass1: lightingPass
            // TODO

            FramebufferLogic::EndRenderPass(context, imageIndex);
        }
    }
}