
#include "glm/glm.hpp"
#include "render/mesh/mesh_comp.h"
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "render/material/material_system.h"
#include "logic/transform/transform_comp.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialInstanceUpdateSystem::Update(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;

        auto meshCache = globalEO->GetComponent<MeshCache>();
        auto materialCache = globalEO->GetComponent<MaterialCache>();

        // reset
        meshCache->currInstance = 0;

        auto &materialEOs = context->renderMaterialEOs;
        for (const auto &materialEO : materialEOs)
        {
            auto material = materialEO->GetComponent<Material>();
            auto useInstance = material->useInstance;

            if (!useInstance)
                continue;

            auto currInstance = meshCache->currInstance++;
            auto &vertexInstance = meshCache->vertexInstances[currInstance];

            auto transform = materialEO->GetComponent<Logic::Transform>();
            vertexInstance.pos = transform->worldPosition;
            vertexInstance.eul = transform->localEulerAngles;
            vertexInstance.scale = 1.0f;

            if (material->data != nullptr)
            {
                auto &instanceDataMap = materialCache->instanceDataMap;
                auto it = instanceDataMap.find(material->info->pipelineName);
                if (it != instanceDataMap.end())
                {
                    instanceDataMap[material->info->pipelineName] = material->data;
                }

                auto &oldData = material->data;
                MaterialLogic::SetDestroy(context, oldData);
            }
        }
    }
}