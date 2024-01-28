
#include "glm/glm.hpp"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/mesh/mesh_system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    inline static void GetOrCreateInstance(Context *context,
                                           std::shared_ptr<Render::Mesh> mesh, bool isShared)
    {
        if (isShared)
        {
            mesh->instance = MeshInstanceLogic::Get(context,
                                                    mesh->info);
        }
        else
        {
            mesh->instance = MeshInstanceLogic::Create(context,
                                                       mesh->info);
        }
    }

    void MeshInstanceCreateSystem::Update(Context *context)
    {
        auto &meshEOs = context->renderMeshEOs;
        for (const auto &meshEO : meshEOs)
        {
            auto mesh = meshEO->GetComponent<Render::Mesh>();
            
            auto &meshInfo = mesh->info;
            if (meshInfo->objName.empty())
                continue;

            auto isShared = MeshLogic::IsShared(context, mesh);

            if (mesh->instance == nullptr)
            {
                GetOrCreateInstance(context, mesh, isShared);
            }

            if (meshInfo->hasChanged)
            {
                if (!isShared)
                    MeshInstanceLogic::SetDestroy(context, mesh->instance);

                GetOrCreateInstance(context, mesh, isShared);
                meshInfo->hasChanged = false;
            }
        }
    }
}