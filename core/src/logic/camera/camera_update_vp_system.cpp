
#include "logic/transform/transform_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "logic/camera/camera_system.h"
#include "context.h"
#include "engine_object.h"

namespace Logic
{
    void CameraUpdateVPSystem::Update(Context *context)
    {
        auto &mainCameraEO = context->logicMainCameraEO;
        auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();

        Logic::CameraLogic::UpdateView(mainCameraEO);
        Logic::CameraLogic::UpdateProjection(context, mainCamera);

        auto &renderLightEOs = context->renderLightEOs;
        for (const auto &lightEO : renderLightEOs)
        {
            auto lightCamera = lightEO->GetComponent<Logic::Camera>();
            Logic::CameraLogic::UpdateView(lightEO);
            Logic::CameraLogic::UpdateProjection(context, lightCamera);
        }
    }
}