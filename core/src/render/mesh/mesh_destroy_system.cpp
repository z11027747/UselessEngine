
#include "glm/glm.hpp"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/mesh/mesh_system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void MeshDestroySystem::Destroy(Context *context)
    {
        auto &meshEOs = context->renderMeshEOs;
        for (const auto &meshEO : meshEOs)
        {
            auto mesh = meshEO->GetComponent<Render::Mesh>();
            if (mesh->vertexColor != glm::vec3(1.0f))
            {
                MeshInstanceLogic::SetDestroy(context, mesh->instance);
            }
        }
    }
}