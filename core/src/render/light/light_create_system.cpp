#pragma once

#include "render/light/light_comp.h"
#include "render/light/light_system.h"
#include "render/light/light_logic.h"
#include "render/light/light_json.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void LightCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(DirectionLight);
        REGIST_ADD_REMOVE(DirectionLight, LightLogic);
        REGIST_JSON(DirectionLight, DirectionLightJson);

        REGIST_CREATE(PointLight);
        REGIST_ADD_REMOVE(PointLight, LightLogic);
        REGIST_JSON(PointLight, PointLightJson);

        REGIST_CREATE(SpotLight);
        REGIST_ADD_REMOVE(SpotLight, LightLogic);
        REGIST_JSON(SpotLight, SpotLightJson);
    }
}