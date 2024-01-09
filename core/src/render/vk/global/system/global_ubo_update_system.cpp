
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_system.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "render/light/light_comp.h"
#include "logic/camera/camera_logic.h"
#include "logic/transform/transform_comp.h"
#include "engine_object.h"
#include "context.h"

class Context;

namespace Render
{
    void GlobalUBOUpdateSystem::Update(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &directionLightEO = context->renderLightEOs[0];
        auto directionLightTransform = directionLightEO->GetComponent<Logic::Transform>();
        auto directionLightCamera = directionLightEO->GetComponent<Logic::Camera>();
        auto directionLight = directionLightEO->GetComponent<Render::DirectionLight>();

        DirectionLightUBO directionLightUBO = {};
        if (directionLightEO->active)
        {
            directionLightUBO = {
                directionLightCamera->view,
                directionLightCamera->projection,
                -directionLightTransform->forward,
                directionLight->color,
                directionLight->params};
        }

        auto &mainCameraEO = context->logicMainCameraEO;
        auto mainCameraTransform = mainCameraEO->GetComponent<Logic::Transform>();
        auto mainCamera = mainCameraEO->GetComponent<Logic::Camera>();

        CameraUBO cameraUBO = {
            mainCameraTransform->position,
            mainCamera->view,
            mainCamera->projection,
        };

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
