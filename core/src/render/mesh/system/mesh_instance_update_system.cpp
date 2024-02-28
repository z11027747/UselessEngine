
#include "glm/glm.hpp"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/mesh/mesh_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    inline static void GetOrCreateInstance(Context *context,
                                           std::shared_ptr<Render::Mesh> mesh, bool isShared)
    {
        mesh->data = (isShared) ? MeshLogic::Get(context, mesh->info) : MeshLogic::Create(context, mesh->info);
    }

    void MeshInstanceUpdateSystem::Update(Context *context)
    {
        auto &meshEOs = context->renderMeshEOs;
        for (const auto &meshEO : meshEOs)
        {
            auto mesh = meshEO->GetComponent<Render::Mesh>();

            auto &meshInfo = mesh->info;
            if (meshInfo->objName.empty())
                continue;

            auto isShared = MeshLogic::IsShared(context, mesh);

            if (mesh->data == nullptr)
            {
                GetOrCreateInstance(context, mesh, isShared);
            }

            if (meshInfo->hasChanged)
            {
                if (!isShared)
                    MeshLogic::SetDestroy(context, mesh->data);

                GetOrCreateInstance(context, mesh, isShared);
                meshInfo->hasChanged = false;
            }
        }
    }
}