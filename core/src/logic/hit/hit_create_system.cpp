#pragma once

#include "logic/hit/hit_comp.h"
#include "logic/hit/hit_system.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void HitCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(HitRay);
    }
}