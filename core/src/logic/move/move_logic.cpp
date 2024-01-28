
#include "logic/move/move_comp.h"
#include "logic/move/move_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Logic
{
    void MoveLogic::BeginFollow(Context *context,
                                std::shared_ptr<EngineObject> selfEO,
                                std::shared_ptr<EngineObject> targetEO, const glm::vec3 &offset)
    {
        std::shared_ptr<MoveFollow> moveFollow;
        if (selfEO->HasComponent<MoveFollow>())
        {
            moveFollow = selfEO->GetComponent<MoveFollow>();
        }
        else
        {
            moveFollow = std::make_shared<MoveFollow>();
            context->AddComponent(selfEO, moveFollow);
        }

        moveFollow->targetEO = targetEO;
        moveFollow->offset = offset;
    }
}