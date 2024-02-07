#pragma once

#include "editor/axis/axis_comp.h"
#include "editor/axis/axis_system.h"
#include "editor/axis/axis_logic.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    void AxisCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(Axis);
        REGIST_ADD_REMOVE(Axis, AxisLogic);
    }
}