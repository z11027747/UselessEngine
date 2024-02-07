
#include "logic/transform/transform_comp.h"
#include "logic/rotate/rotate_logic.h"
#include "logic/rotate/rotate_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void RotateAroundSystem::Update(Context *context)
    {
        auto &rotateEOs = context->logicRotateEOs;
        for (const auto &rotateEO : rotateEOs)
        {
            if (!rotateEO->HasComponent<RotateAround>())
                continue;

            auto rotateAround = rotateEO->GetComponent<RotateAround>();
            auto targetEO = rotateAround->targetEO;

            auto selfTransform = rotateEO->GetComponent<Transform>();
            auto targetTransform = targetEO->GetComponent<Transform>();

            auto offset = selfTransform->worldPosition - targetTransform->worldPosition;

            auto forward = glm::normalize(offset);
            auto right = glm::vec3(forward.z, 0.0f, -forward.x);

            selfTransform->localPosition += right * rotateAround->angleSpeed * context->deltaTime;
        }
    }
}