
#include "render/vk/global/global_comp.h"
#include "render/vk/logic.h"
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/render_pass/render_pass_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_logic.h"
#include "editor/test_logic.h"
#include "editor/system.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
    void TestLogic::CreateMainCamera(Context *context)
    {
        auto mainCameraEO = std::make_shared<EngineObject>();
        mainCameraEO->name = Name_MainCamera;

        Logic::TransformLogic::Add(mainCameraEO,
                                   glm::vec3(0.7f, 1.49743f, -3.979505f),
                                   glm::vec3(-1.0f, 0.0f, 0.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f));

        auto mainCamera = std::make_shared<Logic::Camera>();
        mainCamera->near = 0.1f;
        mainCamera->far = 100.0f;
        mainCamera->mode = Logic::CameraMode::ePerspective;
        mainCamera->fov = 60.0f;
        mainCamera->passName = Render::Pass_Main;
        mainCameraEO->AddComponent<Logic::Camera>(mainCamera);

        context->AddEO(mainCameraEO);
    }

    void TestLogic::CreateLight(Context *context)
    {
        auto directionLightEO = std::make_shared<EngineObject>();
        directionLightEO->name = Name_DirectionLight;

        Logic::TransformLogic::Add(directionLightEO,
                                   glm::vec3(-5.0f, 5.0f, 0.0f),
                                   glm::vec3(45.0f, 45.0f, 0.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f));

        auto directionLight = std::make_shared<Render::DirectionLight>();
        directionLight->color = glm::vec3(1.0f, 0.9568627f, 0.8392157f);
        directionLight->hasShadow = true;

        directionLightEO->AddComponent(directionLight);

        auto directionLightCamera = std::make_shared<Logic::Camera>();
        directionLightCamera->near = 0.1f;
        directionLightCamera->far = 50.0f;
        directionLightCamera->mode = Logic::CameraMode::eOrtho;
        directionLightCamera->size = 30.0f;
        directionLightCamera->passName = Render::Pass_Shadow;
        directionLightEO->AddComponent<Logic::Camera>(directionLightCamera);

        context->AddEO(directionLightEO);
    }

    void TestLogic::CreateAxis(Context *context)
    {
        auto modelParentEO = std::make_shared<EngineObject>();
        modelParentEO->name = Name_Axis;
        modelParentEO->active = false;
        modelParentEO->hideInHierarchy = true;

        Logic::TransformLogic::Add(modelParentEO,
                                   glm::vec3(1.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(0.5f, 0.5f, 0.5f));

        context->AddEO(modelParentEO);

        {
            auto modelEO = std::make_shared<EngineObject>();
            modelEO->name = Name_AxisX;
            modelEO->hideInHierarchy = true;

            Logic::TransformLogic::Add(modelEO,
                                       glm::vec3(1.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 90.0f, 0.0f),
                                       glm::vec3(1.0f, 1.0f, 1.0f));
            Logic::TransformLogic::SetParent(modelEO, modelParentEO);

            auto modelMesh = std::make_shared<Render::Mesh>();
            modelMesh->info = std::make_shared<Render::MeshInfo>();
            modelMesh->info->objName = "resource/obj/base/axis.obj";
            modelMesh->info->vertexColor = glm::vec3(1.0f, 0.0f, 0.0f);
            modelMesh->info->checkHit = true;

            auto modelMaterial = std::make_shared<Render::Material>();
            modelMaterial->info = std::make_shared<Render::MaterialInfo>();
            modelMaterial->info->pipelineName = Render::Pipeline_Color;
            modelMaterial->info->renderQueue = Render::Queue_Geometry + 50;

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);

            context->AddEO(modelEO);
        }

        {
            auto modelEO = std::make_shared<EngineObject>();
            modelEO->name = Name_AxisY;
            modelEO->hideInHierarchy = true;

            Logic::TransformLogic::Add(modelEO,
                                       glm::vec3(0.0f, 1.0f, 0.0f),
                                       glm::vec3(-90.0f, 0.0f, 0.0f),
                                       glm::vec3(1.0f, 1.0f, 1.0f));
            Logic::TransformLogic::SetParent(modelEO, modelParentEO);

            auto modelMesh = std::make_shared<Render::Mesh>();
            modelMesh->info = std::make_shared<Render::MeshInfo>();
            modelMesh->info->objName = "resource/obj/base/axis.obj";
            modelMesh->info->vertexColor = glm::vec3(0.0f, 1.0f, 0.0f);
            modelMesh->info->checkHit = true;

            auto modelMaterial = std::make_shared<Render::Material>();
            modelMaterial->info = std::make_shared<Render::MaterialInfo>();
            modelMaterial->info->pipelineName = Render::Pipeline_Color;
            modelMaterial->info->renderQueue = Render::Queue_Geometry + 50;

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);

            context->AddEO(modelEO);
        }

        {
            auto modelEO = std::make_shared<EngineObject>();
            modelEO->name = Name_AxisZ;
            modelEO->hideInHierarchy = true;

            Logic::TransformLogic::Add(modelEO,
                                       glm::vec3(0.0f, 0.0f, 1.0f),
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(1.0f, 1.0f, 1.0f));
            Logic::TransformLogic::SetParent(modelEO, modelParentEO);

            auto modelMesh = std::make_shared<Render::Mesh>();
            modelMesh->info = std::make_shared<Render::MeshInfo>();
            modelMesh->info->objName = "resource/obj/base/axis.obj";
            modelMesh->info->vertexColor = glm::vec3(0.0f, 0.0f, 1.0f);
            modelMesh->info->checkHit = true;

            auto modelMaterial = std::make_shared<Render::Material>();
            modelMaterial->info = std::make_shared<Render::MaterialInfo>();
            modelMaterial->info->pipelineName = Render::Pipeline_Color;
            modelMaterial->info->renderQueue = Render::Queue_Geometry + 50;

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);

            context->AddEO(modelEO);
        }
    }
}
