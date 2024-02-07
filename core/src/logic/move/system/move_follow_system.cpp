
#include "logic/transform/transform_comp.h"
#include "logic/move/move_comp.h"
#include "logic/move/move_logic.h"
#include "logic/move/move_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void MoveFollowSystem::Update(Context *context)
    {
        auto &logicMoveEOs = context->logicMoveEOs;
        for (const auto &moveEO : logicMoveEOs)
        {
            auto transform = moveEO->GetComponent<Transform>();
            auto moveFollow = moveEO->GetComponent<MoveFollow>();
            auto &offset = moveFollow->offset;

            auto targetEO = moveFollow->targetEO;
            if (targetEO != nullptr)
            {
                auto targetTransform = targetEO->GetComponent<Transform>();
                if (targetTransform != nullptr)
                {
                    transform->localPosition = targetTransform->worldPosition + offset;
                    continue;
                }
            }

            // invalid
            context->RemoveComponent<MoveFollow>(moveEO);
        }
    }
}