
#include "render/post_process/post_process_logic.h"
#include "logic/camera/camera_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Logic
{
    void CameraLogic::OnAdd(Context *context,
                            std::shared_ptr<EngineObject> eo)
    {
        if (eo->name == Define::EOName::MainCamera)
        {
            context->logicMainCameraEO = eo;
            Render::PostProcessLogic::Create(context, eo);
        }
    }
    void CameraLogic::OnRemove(Context *context,
                               std::shared_ptr<EngineObject> eo)
    {
        if (eo->name == Define::EOName::MainCamera)
        {
            Render::PostProcessLogic::Create(context, eo);
            context->logicMainCameraEO = nullptr;
        }
    }
}