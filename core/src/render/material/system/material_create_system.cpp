#pragma once

#include "render/material/material_comp.h"
#include "render/material/material_system.h"
#include "render/material/material_logic.h"
#include "render/material/material_json.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(Material);
        REGIST_ADD_REMOVE(Material, MaterialLogic);
        REGIST_JSON(Material, MaterialJson);
    }
}