
#include "render/vk/global/global_comp.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/material/material_system.h"
#include "logic/camera/camera_logic.h"
#include "logic/transform/transform_comp.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    void MaterialGlobalUBOUpdateSystem::Update(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        DirectionLightUBO directionLightUBO = {};

        if (context->renderLightEOs.size() > 0)
        {
            auto &directionLightEO = context->renderLightEOs[0];
            if (directionLightEO->active)
            {
                auto directionLightTransform = directionLightEO->GetComponent<Logic::Transform>();
                auto directionLightCamera = directionLightEO->GetComponent<Logic::Camera>();
                auto directionLight = directionLightEO->GetComponent<Render::DirectionLight>();

                directionLightUBO = {
                    -directionLightTransform->forward,
                    directionLightCamera->view,
                    directionLightCamera->projection,
                    directionLight->ambient,
                    directionLight->color,
                    directionLight->params};
            }
        }

        CameraUBO cameraUBO = {};

        auto &mainCameraEO = context->logicMainCameraEO;
        if (mainCameraEO != nullptr)
        {
            auto mainCameraTransform = mainCameraEO->GetComponent<Logic::Transform>();
            auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();

            cameraUBO = {
                mainCameraTransform->localPosition,
                mainCamera->view,
                mainCamera->projection,
            };
        }

        GlobalUBO globalUBO = {
            cameraUBO,
            directionLightUBO};

        auto &globalDescriptor = global->globalDescriptor;
        auto &globalBuffer = global->globalBuffer;

        BufferSetLogic::Set(context,
                            globalBuffer,
                            globalUBO);
    }
}
