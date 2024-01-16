
#include "glm/glm.hpp"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/mesh/mesh_system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void MeshInstanceCreateSystem::Update(Context *context)
    {
        auto &meshEOs = context->renderMeshEOs;
        for (const auto &meshEO : meshEOs)
        {
            auto mesh = meshEO->GetComponent<Render::Mesh>();

            if (mesh->instance == nullptr)
            {
                auto isShared = (mesh->vertexColor == glm::vec3(1.0f));
                mesh->instance = isShared
                                     ? MeshInstanceLogic::Get(context, mesh->objName)
                                     : MeshInstanceLogic::Create(context, mesh->objName, mesh->vertexColor);
            }
            else if (mesh->objName != mesh->instance->objName)
            {
                auto isShared = (mesh->vertexColor == glm::vec3(1.0f));
                if (!isShared)
                {
                    MeshInstanceLogic::SetDestroy(context, mesh->instance);

                    mesh->instance = MeshInstanceLogic::Create(context, mesh->objName, mesh->vertexColor);
                }
                else
                {
                    mesh->instance = MeshInstanceLogic::Get(context, mesh->objName);
                }
            }
        }
    }
}