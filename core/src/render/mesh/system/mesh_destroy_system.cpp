
#include "glm/glm.hpp"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/mesh/mesh_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MeshDestroySystem::Destroy(Context *context)
    {
        auto &meshEOs = context->renderMeshEOs;
        for (const auto &meshEO : meshEOs)
        {
            MeshLogic::TryDestroyEO(context, meshEO);
        }
        meshEOs.clear();

        MeshLogic::DestroyCache(context);
    }
}