
#include "logic/transform/transform_comp.h"
#include "logic/move/move_logic.h"
#include "logic/move/move_system.h"
#include "engine_object.h"
#include "context.h"

namespace Logic
{
    static void UpdateMoveFollow(Context *context,
                                 std::shared_ptr<EngineObject> eo)
    {
        auto moveFollow = eo->GetComponent<MoveFollow>();
        auto targetEO = moveFollow->targetEO;
        auto &offset = moveFollow->offset;

        auto transform = eo->GetComponent<Transform>();
        auto targetTransform = targetEO->GetComponent<Transform>();

        transform->localPosition = targetTransform->localPosition + offset;
    }

    void MoveFollowSystem::Update(Context *context)
    {
        auto &logicMoveEOs = context->logicMoveEOs;
        for (const auto &moveEO : logicMoveEOs)
        {
            if (moveEO->HasComponent<MoveFollow>())
            {
                UpdateMoveFollow(context, moveEO);
            }
        }
    }
}