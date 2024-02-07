
#include "logic/move/move_comp.h"
#include "logic/move/move_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void MoveLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->logicMoveEOs.push_back(eo);
    }

    void MoveLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        context->RemoveEOInVector(eo, context->logicMoveEOs);
    }
}