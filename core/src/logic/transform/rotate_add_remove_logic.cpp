
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void RotateLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        // TODO 
        auto targetEO = context->GetEO("Plane");
        BeginRotateAround(context,
                          eo,
                          targetEO, 10);

        context->logicRotateEOs.push_back(eo);
    }

    void RotateLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->RemoveEOInVector(eo, context->logicRotateEOs);
    }
}