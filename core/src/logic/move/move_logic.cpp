
#include "logic/move/move_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Logic
{
    void MoveLogic::BeginFollow(Context *context,
                                std::shared_ptr<EngineObject> selfEO,
                                std::shared_ptr<EngineObject> targetEO, glm::vec3 &offset)
    {
        std::shared_ptr<MoveFollow> moveFollow;
        if (selfEO->HasComponent<MoveFollow>())
        {
            moveFollow = selfEO->GetComponent<MoveFollow>();
        }
        else
        {
            moveFollow = std::make_shared<MoveFollow>();
            selfEO->AddComponent(moveFollow);
            context->logicMoveEOs.push_back(selfEO);
        }

        moveFollow->targetEO = targetEO;
        moveFollow->offset = offset;
    }
}