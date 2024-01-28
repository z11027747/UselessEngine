
#include "logic/camera/camera_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Logic
{
    void CameraLogic::OnAdd(Context *context,
                            std::shared_ptr<EngineObject> eo)
    {
        if (eo->name == Define::EOName::MainCamera)
            context->logicMainCameraEO = eo;
    }
    void CameraLogic::OnRemove(Context *context,
                               std::shared_ptr<EngineObject> eo)
    {
        if (eo->name == Define::EOName::MainCamera)
            context->logicMainCameraEO = nullptr;
    }
}