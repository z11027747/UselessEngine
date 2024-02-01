
#include "glm/glm.hpp"
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "render/material/material_system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void MaterialInstanceCreateSystem::Update(Context *context)
    {
        auto &materialEOs = context->renderMaterialEOs;
        for (const auto &materialEO : materialEOs)
        {
            auto material = materialEO->GetComponent<Render::Material>();

            auto &info = material->info;
            if (info->pipelineName.empty())
                continue;

            if (material->instance == nullptr)
            {
                material->instance = MaterialInstanceLogic::Create(context, info);
            }

            if (info->hasChanged)
            {
                auto &oldInstance = material->instance;
                oldInstance->info = nullptr;
                MaterialInstanceLogic::SetDestroy(context, oldInstance);

                material->instance = MaterialInstanceLogic::Create(context,
                                                                   info);

                info->hasChanged = false;
            }
        }
    }
}