#pragma once

#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_system.h"
#include "logic/transform/transform_logic.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void MoveCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(MoveFowrard);
        REGIST_ADD_REMOVE(MoveFowrard, MoveLogic);
        
        REGIST_CREATE(MoveFollow);
        REGIST_ADD_REMOVE(MoveFollow, MoveLogic);
    }
}