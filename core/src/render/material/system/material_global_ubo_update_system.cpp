
#include "render/vk/global/global_comp.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/material/material_system.h"
#include "logic/camera/camera_logic.h"
#include "logic/transform/transform_comp.h"
#include "engine_object.hpp"
#include "context.hpp"

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
                mainCameraTransform->worldPosition,
                mainCamera->view,
                mainCamera->projection,
            };
        }

        DirectionLightUBO directionLightUBO = {};
        PointLightUBO pointLightUBOs[256];
        int activePointLights = 0;
        SpotLightUBO spotLightUBOs[4];
        int activeSpotLights = 0;

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
                    directionLightTransform->forward,
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
                    // pointLightCamera->view,
                    // pointLightCamera->projection,
                    pointLight->color,
                    pointLight->clq,
                };
                activePointLights++;
            }

            if (lightEO->HasComponent<Render::SpotLight>())
            {
                auto spotLightTransform = lightEO->GetComponent<Logic::Transform>();
                // auto spotLightCamera = lightEO->GetComponent<Logic::Camera>();
                auto spotLight = lightEO->GetComponent<Render::SpotLight>();

                spotLightUBOs[activeSpotLights] = {
                    spotLightTransform->forward,
                    spotLightTransform->worldPosition,
                    // spotLightCamera->view,
                    // spotLightCamera->projection,
                    spotLight->color,
                    spotLight->cutOff,
                };
                activeSpotLights++;
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
        for (auto i = 0; i < activeSpotLights; i++)
        {
            globalUBO.spotLights[i] = spotLightUBOs[i];
        }
        globalUBO.activeSpotLights = activeSpotLights;
        globalUBO.currTime = context->currTime;

        auto &globalBuffer = global->globalBuffer;
        BufferSetLogic::Set(context,
                            globalBuffer,
                            globalUBO);
    }
}
