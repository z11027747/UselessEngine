
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
            if (material->instance == nullptr)
            {
                material->instance = MaterialInstanceLogic::Get(context,
                                                                material->pipelineName, material->image0Names);
            }

            if (material->hasChanged)
            {
                auto newInstance = MaterialInstanceLogic::Create(context,
                                                                 material->pipelineName, material->image0Names);

                auto &oldInstance = material->instance;
                auto oldPipeline = oldInstance->pipelineName;
                auto oldImage0 = oldInstance->image0;
                auto oldDescriptor = oldInstance->descriptor;

                oldInstance->pipelineName = newInstance->pipelineName;
                oldInstance->image0 = newInstance->image0;
                oldInstance->descriptor = newInstance->descriptor;

                newInstance->pipelineName = oldPipeline;
                newInstance->image0 = oldImage0;
                newInstance->descriptor = oldDescriptor;

                MaterialInstanceLogic::SetDestroy(context, newInstance);

                material->hasChanged = false;
            }

            if (material->image0Names[0] != material->instance->image0Names[0])
            {
                material->image0Names = material->instance->image0Names;
            }
        }
    }
}