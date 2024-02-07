#pragma once

#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_system.h"
#include "render/mesh/mesh_logic.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MeshCreateSystem::Create(Context *context)
    {
        REGIST_CREATE(Mesh);
        REGIST_ADD_REMOVE(Mesh, MeshLogic);
        REGIST_JSON(Mesh, MeshJson);
    }
}