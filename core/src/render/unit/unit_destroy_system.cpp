#pragma once

#include "render/mesh/mesh_logic.h"
#include "render/material/material_logic.h"
#include "render/unit/unit_comp.h"
#include "render/unit/unit_system.h"
#include "context.h"

namespace Render
{
    void UnitDestroySystem::Destroy(Context *context)
    {
        auto &unitEOs = context->renderUnitEOs;
        for (const auto &unitEO : unitEOs)
        {
            auto mesh = unitEO->GetComponent<Mesh>();
            auto material = unitEO->GetComponent<Material>();

            MeshLogic::Destroy(context, mesh);
            MaterialLogic::Destroy(context, material);
        }
        unitEOs.clear();
    }
};