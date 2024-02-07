#pragma once

#include "render/post_process/post_process_comp.h"
#include "render/post_process/post_process_system.h"
#include "render/post_process/post_process_logic.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void PostProcessCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(PostProcess);
        REGIST_ADD_REMOVE(PostProcess, PostProcessLogic);
        REGIST_JSON(PostProcess, PostProcessJson);
    }
}