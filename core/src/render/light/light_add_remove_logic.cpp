
#pragma once

#include "render/light/light_comp.h"
#include "render/light/light_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

class Context;
class EngineObject;

namespace Render
{
    void LightLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->renderLightEOs.push_back(eo);
    }
    void LightLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->RemoveEOInVector(eo, context->renderLightEOs);
    }
}