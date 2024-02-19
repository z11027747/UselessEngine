#pragma once

#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_system.h"
#include "logic/transform/transform_logic.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void ScaleCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(ScaleKeep);
        REGIST_ADD_REMOVE(ScaleKeep, ScaleLogic);
        
        REGIST_CREATE(ScaleKeep);
        REGIST_ADD_REMOVE(ScaleKeep, ScaleLogic);
    }
}