
#include "glm/glm.hpp"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/mesh/mesh_system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    inline static void SetInstance(Context *context,
                                   std::shared_ptr<Render::Mesh> mesh, bool isShared)
    {
        if (isShared)
        {
            mesh->instance = MeshInstanceLogic::Get(context,
                                                    mesh->objName);
        }
        else
        {
            mesh->instance = MeshInstanceLogic::Create(context,
                                                       mesh->objName, mesh->vertexColor);
        }
    }

    void MeshInstanceCreateSystem::Update(Context *context)
    {
        auto &meshEOs = context->renderMeshEOs;
        for (const auto &meshEO : meshEOs)
        {
            auto mesh = meshEO->GetComponent<Render::Mesh>();
            if (mesh->objName.empty())
                continue;

            auto isShared = MeshLogic::IsShared(context, mesh);

            if (mesh->instance == nullptr)
            {
                SetInstance(context, mesh, isShared);
            }

            if (mesh->hasChanged)
            {
                if (!isShared)
                    MeshInstanceLogic::SetDestroy(context, mesh->instance);

                SetInstance(context, mesh, isShared);
                mesh->hasChanged = false;
            }
        }
    }
}