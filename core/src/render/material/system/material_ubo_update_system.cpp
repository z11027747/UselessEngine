
#include "render/vk/global/global_comp.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "render/material/material_comp.h"
#include "render/material/material_system.h"
#include "render/material/material_logic.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    void MaterialUBOUpdateSystem::Update(Context *context)
    {
        auto &materialEOs = context->renderMaterialEOs;
        for (auto &materialEO : materialEOs)
        {
            if (!materialEO->active)
                continue;

            auto material = materialEO->GetComponent<Material>();
            auto &materialInfo = material->info;

            if (materialInfo->params.empty())
                continue;

            MaterialUBO materialUBO = {
                materialInfo->params[0]};

            auto &buffer = material->instance->buffer;
            BufferSetLogic::Set(context,
                                buffer,
                                materialUBO);
        }
    }
}
