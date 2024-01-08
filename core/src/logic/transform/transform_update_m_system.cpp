
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/transform/transform_system.h"
#include "context.h"
#include "engine_object.h"

namespace Logic
{
    void TransformUpdateMSystem::Update(Context *context)
    {
        auto &allEOs = context->allEOs;
        for (const auto &eo : allEOs)
        {
            auto hasTransform = eo->HasComponent<Transform>();
            if (hasTransform)
                TransformLogic::UpdateModel(eo);
        }
    }
}