#pragma once

#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/transform/transform_system.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void RotateCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(RotateAround);
        REGIST_ADD_REMOVE(RotateAround, RotateLogic);
    }
}