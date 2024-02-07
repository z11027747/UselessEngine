
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_comp.h"
#include "logic/scene/scene_logic.h"
#include "editor/axis/axis_comp.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void SceneLogic::CreateMainCamera(Context *context)
    {
        auto mainCameraEO = std::make_shared<EngineObject>();
        mainCameraEO->name = Define::EOName::MainCamera;

        Logic::TransformLogic::Add(mainCameraEO,
                                   glm::vec3(0.7f, 1.49743f, -3.979505f),
                                   glm::vec3(-1.0f, 0.0f, 0.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f));

        auto mainCamera = std::make_shared<Logic::Camera>();
        mainCamera->near = 0.1f;
        mainCamera->far = 100.0f;
        mainCamera->mode = Logic::CameraMode::ePerspective;
        mainCamera->fov = 60.0f;
        mainCamera->passName = Define::Pass::Forward;
        context->AddComponent(mainCameraEO, mainCamera);

        context->AddEO(mainCameraEO);
    }

    static void CreateAxis_Single(Context *context,
                                  const std::string &name,
                                  glm::vec3 pos, glm::vec3 eul,
                                  std::shared_ptr<EngineObject> parentEO,
                                  glm::vec4 param)
    {
        auto eo = std::make_shared<EngineObject>();
        eo->name = name;
        eo->hideInHierarchy = true;
        eo->save = false;
        eo->dontDestroy = true;

        Logic::TransformLogic::Add(eo, pos, eul);
        Logic::TransformLogic::SetParent(eo, parentEO);

        auto mesh = std::make_shared<Render::Mesh>();
        mesh->info = std::make_shared<Render::MeshInfo>();
        mesh->info->objName = "resource/obj/base/axis.obj";

        auto material = std::make_shared<Render::Material>();
        material->info = std::make_shared<Render::MaterialInfo>();
        material->info->pipelineName = Define::Pipeline::Color;
        material->info->params = {param};
        material->info->renderQueue = Define::RenderQueue::Geometry + 50;

        auto axis = std::make_shared<Editor::Axis>();

        context->AddComponent(eo, mesh);
        context->AddComponent(eo, material);
        context->AddComponent(eo, axis);

        context->AddEO(eo);
    }

    void SceneLogic::CreateAxis(Context *context)
    {
        auto parentEO = std::make_shared<EngineObject>();
        parentEO->name = Define::EOName::Axis;
        parentEO->active = false;
        parentEO->hideInHierarchy = true;
        parentEO->save = false;
        parentEO->dontDestroy = true;

        Logic::TransformLogic::Add(parentEO,
                                   glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.5f));

        context->AddEO(parentEO);

        // x
        CreateAxis_Single(context,
                          Define::EOName::AxisX,
                          glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 90.0f, 0.0f),
                          parentEO,
                          glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        // y
        CreateAxis_Single(context,
                          Define::EOName::AxisY,
                          glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-90.0f, 0.0f, 0.0f),
                          parentEO,
                          glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        // z
        CreateAxis_Single(context,
                          Define::EOName::AxisZ,
                          glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                          parentEO,
                          glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    }
}
