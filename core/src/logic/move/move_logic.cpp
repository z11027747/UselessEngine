
#include "logic/move/move_comp.h"
#include "logic/move/move_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Logic
{
    void MoveLogic::BeginFollow(Context *context,
                                std::shared_ptr<EngineObject> selfEO,
                                std::shared_ptr<EngineObject> targetEO, glm::vec3 &offset)
    {
        auto moveFollow = std::make_shared<MoveFollow>();
        moveFollow->targetEO = targetEO;
        moveFollow->offset = offset;

        selfEO->AddComponent(moveFollow);
        context->logicMoveEOs.emplace_back(selfEO);
    }
}