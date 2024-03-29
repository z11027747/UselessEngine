
#include "render/vk/pipeline/pipeline_logic.h"
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "render/material/material_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialDestroySystem::Destroy(Context *context)
    {
        auto &materialEOs = context->renderMaterialEOs;
        for (const auto &materialEO : materialEOs)
        {
            MaterialLogic::TryDestroyEO(context, materialEO);
        }
        materialEOs.clear();

        MaterialLogic::DestroyCache(context);
        
        MaterialGlobalDescriptorLogic::Destroy(context);

        PipelineLogic::DestroyAll(context);
    }
}