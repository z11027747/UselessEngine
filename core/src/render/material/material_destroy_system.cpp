
#include "glm/glm.hpp"
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "render/material/material_system.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void MaterialDestroySystem::Destroy(Context *context)
    {
        auto &materialEOs = context->renderMaterialEOs;
        for (const auto &materialEO : materialEOs)
        {
        }
    }
}