
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
                    auto newInstance = MaterialInstanceLogic::Create(context,
                                                                     material->pipelineName,
                                                                     material->imageNames, material->isImageCube);

                    auto &oldInstance = material->instance;
                    auto oldPipeline = oldInstance->pipelineName;
                    auto oldImages = oldInstance->images;
                    auto oldDescriptor = oldInstance->descriptor;

                    oldInstance->pipelineName = newInstance->pipelineName;
                    oldInstance->images = newInstance->images;
                    oldInstance->descriptor = newInstance->descriptor;

                    newInstance->pipelineName = oldPipeline;
                    newInstance->images = oldImages;
                    newInstance->descriptor = oldDescriptor;

                    MaterialInstanceLogic::SetDestroy(context, newInstance);
                }

                material->hasChanged = false;
            }

            if (material->imageNames.size() > 0 &&
                material->imageNames[0] != material->instance->imageNames[0])
            {
                material->imageNames = material->instance->imageNames;
            }
        }
    }
}