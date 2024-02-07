
#include "logic/scene/scene_system.h"
#include "logic/scene/scene_logic.h"
#include "define.hpp"
#include "context.hpp"
#include "engine_object.hpp"

namespace Logic
{
    void SceneCreateSystem::Create(Context *context)
    {
        SceneLogic::CreateMainCamera(context);
        // SceneLogic::CreateSkybox(context);
        SceneLogic::CreateAxis(context);
    }
}
