
#pragma once

#include "render/light/light_comp.h"
#include "render/light/light_logic.h"
#include "engine_object.h"
#include "context.h"

// Temp
#include "logic/rotate/rotate_comp.h"

class Context;
class EngineObject;

namespace Render
{
    void LightLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->renderLightEOs.push_back(eo);

        if (eo->HasComponent<PointLight>())
        {
            auto rotateAround = std::make_shared<Logic::RotateAround>();
            context->AddComponent(eo, rotateAround);
        }
    }
    void LightLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->RemoveEOInVector(eo, context->renderLightEOs);
    }
}