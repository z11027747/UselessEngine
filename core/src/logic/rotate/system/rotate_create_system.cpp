#pragma once

#include "logic/rotate/rotate_comp.h"
#include "logic/rotate/rotate_system.h"
#include "logic/rotate/rotate_logic.h"
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