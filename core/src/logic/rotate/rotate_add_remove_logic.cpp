
#include "logic/rotate/rotate_comp.h"
#include "logic/rotate/rotate_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Logic
{
    void RotateLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        // TODO 临时放在这
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