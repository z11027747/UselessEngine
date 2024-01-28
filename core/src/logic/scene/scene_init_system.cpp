
#include "logic/scene/scene_system.h"
#include "logic/scene/scene_logic.h"
#include "common/define.h"
#include "context.h"
#include "engine_object.h"

namespace Logic
{
    void SceneInitSystem::Create(Context *context)
    {
        SceneLogic::CreateMainCamera(context);
        // SceneLogic::CreateSkybox(context);
        SceneLogic::CreateAxis(context);
    }

}
