
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->renderMaterialEOs.push_back(eo);
    }

    void MaterialLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        TryDestroyEO(context, eo);

        context->RemoveEOInVector(eo, context->renderMaterialEOs);
    }
}