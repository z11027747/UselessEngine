
#include "glm/glm.hpp"
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "render/material/material_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialInstanceUpdateSystem::Update(Context *context)
    {
        auto &materialEOs = context->renderMaterialEOs;
        for (const auto &materialEO : materialEOs)
        {
            auto material = materialEO->GetComponent<Render::Material>();

            auto &info = material->info;
            if (info->pipelineName.empty())
                continue;

            if (material->data == nullptr)
            {
                material->data = MaterialLogic::Create(context, info);
            }

            if (info->hasChanged)
            {
                auto &oldData = material->data;
                oldData->info = nullptr;
                MaterialLogic::SetDestroy(context, oldData);

                material->data = MaterialLogic::Create(context, info);

                info->hasChanged = false;
            }
        }
    }
}