
#include "logic/transform/transform_comp.h"
#include "logic/rotate/rotate_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Logic
{
    void RotateLogic::BeginRotateAround(Context *context,
                                        std::shared_ptr<EngineObject> selfEO,
                                        std::shared_ptr<EngineObject> targetEO, float angleSpeed)
    {
        std::shared_ptr<RotateAround> rotateAround;
        if (selfEO->HasComponent<RotateAround>())
        {
            rotateAround = selfEO->GetComponent<RotateAround>();
        }
        else
        {
            rotateAround = std::make_shared<RotateAround>();
            selfEO->AddComponent(rotateAround);

            context->logicRotateEOs.push_back(selfEO);
        }

        auto selfTransform = selfEO->GetComponent<Transform>();
        auto targetTransform = targetEO->GetComponent<Transform>();
        auto offset = selfTransform->worldPosition - selfTransform->worldPosition;

        rotateAround->targetEO = targetEO;
        rotateAround->offset = offset;
        rotateAround->angleSpeed = angleSpeed;
        rotateAround->currAngle = 0.0f;
    }

    void RotateLogic::BeginRotateAround(Context *context,
                                        std::shared_ptr<EngineObject> selfEO,
                                        std::shared_ptr<EngineObject> targetEO, const glm::vec3 &offset, float angleSpeed)
    {
        std::shared_ptr<RotateAround> rotateAround;
        if (selfEO->HasComponent<RotateAround>())
        {
            rotateAround = selfEO->GetComponent<RotateAround>();
        }
        else
        {
            rotateAround = std::make_shared<RotateAround>();
            selfEO->AddComponent(rotateAround);

            context->logicRotateEOs.push_back(selfEO);
        }

        rotateAround->targetEO = targetEO;
        rotateAround->offset = offset;
        rotateAround->angleSpeed = angleSpeed;
        rotateAround->currAngle = 0.0f;
    }
}