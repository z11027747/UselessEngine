#pragma once

#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_system.h"
#include "logic/transform/transform_logic.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void TransformCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(Transform);
        REGIST_JSON(Transform, TransformJson);
    }
}