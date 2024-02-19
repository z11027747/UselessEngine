#pragma once

#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_system.h"
#include "logic/transform/transform_logic.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void ScaleKeepSystem::Update(Context *context)
    {
        auto &cameraEO = context->logicMainCameraEO;

        auto &scaleEOs = context->logicScaleEOs;
        for (const auto &scaleEO : scaleEOs)
        {
            if (!scaleEO->HasComponent<ScaleKeep>())
                continue;

            auto scaleKeep = scaleEO->GetComponent<ScaleKeep>();
            auto &initDistance = scaleKeep->initDistance;

            auto currDistance = TransformLogic::Distance(scaleEO, cameraEO);
            auto currScale = currDistance / initDistance;

            auto scaleTransform = scaleEO->GetComponent<Transform>();
            scaleTransform->localScale = glm::vec3(currScale);
        }
    }
}