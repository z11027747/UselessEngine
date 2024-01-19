
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
            if (material->pipelineName.empty())
                continue;

            if (material->instance == nullptr)
            {
                material->instance = MaterialInstanceLogic::Get(context,
                                                                material->pipelineName,
                                                                material->imageNames, material->isImageCube);
            }

            if (material->hasChanged)
            {
                if (material->pipelineName != material->instance->pipelineName)
                {
                    material->instance = MaterialInstanceLogic::Get(context,
                                                                    material->pipelineName,
                                                                    material->imageNames, material->isImageCube);
                }
                else
                {
                    // TODO
                    auto &oldInstance = material->instance;
                    MaterialInstanceLogic::SetDestroy(context, oldInstance);

                    material->instance = MaterialInstanceLogic::Create(context,
                                                                       material->pipelineName,
                                                                       material->imageNames, material->isImageCube);
                }

                material->hasChanged = false;
            }
        }
    }
}