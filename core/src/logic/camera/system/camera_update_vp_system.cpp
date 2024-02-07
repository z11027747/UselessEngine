
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_logic.h"
#include "logic/camera/camera_system.h"
#include "context.hpp"
#include "engine_object.hpp"

namespace Logic
{
    void CameraUpdateVPSystem::Update(Context *context)
    {
        auto &mainCameraEO = context->logicMainCameraEO;
        if (mainCameraEO == nullptr)
            return;

        auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();

        Logic::CameraLogic::UpdateView(mainCameraEO);
        Logic::CameraLogic::UpdateProjection(context, mainCamera);

        auto &renderLightEOs = context->renderLightEOs;
        for (const auto &lightEO : renderLightEOs)
        {
            // TODO 此处正常应该都有Camera
            if (!lightEO->HasComponent<Logic::Camera>())
                continue;

            auto lightCamera = lightEO->GetComponent<Logic::Camera>();
            Logic::CameraLogic::UpdateView(lightEO);
            Logic::CameraLogic::UpdateProjection(context, lightCamera);
        }
    }
}