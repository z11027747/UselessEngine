
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "render/material/material_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void MaterialLogic::TryDestroyEO(Context *context, std::shared_ptr<EngineObject> eo)
    {
        auto material = eo->GetComponent<Render::Material>();
        MaterialInstanceLogic::SetDestroy(context, material->instance);
    }
}