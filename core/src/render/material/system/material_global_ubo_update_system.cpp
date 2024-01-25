
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

        DirectionLightUBO directionLightUBO = {};
        PointLightUBO pointLightUBOs[4];
        int activePointLights = 0;

        auto &lightEOs = context->renderLightEOs;
        auto lightEOSize = lightEOs.size();
        for (auto i = 0; i < lightEOSize; i++)
        {
            auto &lightEO = lightEOs[i];
            if (!lightEO->active)
                continue;

            if (lightEO->HasComponent<Render::DirectionLight>())
            {
                auto directionLightTransform = lightEO->GetComponent<Logic::Transform>();
                auto directionLightCamera = lightEO->GetComponent<Logic::Camera>();
                auto directionLight = lightEO->GetComponent<Render::DirectionLight>();

                directionLightUBO = {
                    -directionLightTransform->forward,
                    directionLightCamera->view,
                    directionLightCamera->projection,
                    directionLight->ambient,
                    directionLight->color};
            }

            if (lightEO->HasComponent<Render::PointLight>())
            {
                auto pointLightTransform = lightEO->GetComponent<Logic::Transform>();
                // auto pointLightCamera = lightEO->GetComponent<Logic::Camera>();
                auto pointLight = lightEO->GetComponent<Render::PointLight>();

                pointLightUBOs[activePointLights] = {
                    pointLightTransform->worldPosition,
                    glm::mat4(1.0f), // pointLightCamera->view,
                    glm::mat4(1.0f), // pointLightCamera->projection,
                    pointLight->color,
                    pointLight->clq,
                };
                activePointLights++;
            }
        }

        GlobalUBO globalUBO = {};
        globalUBO.camera = cameraUBO;
        globalUBO.directionLight = directionLightUBO;
        for (auto i = 0; i < activePointLights; i++)
        {
            globalUBO.pointLights[i] = pointLightUBOs[i];
        }
        globalUBO.activePointLights = activePointLights;

        auto &globalBuffer = global->globalBuffer;
        BufferSetLogic::Set(context,
                            globalBuffer,
                            globalUBO);
    }
}
