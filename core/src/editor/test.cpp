
#include <vector>
#include <string>
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/material/material_comp.h"
#include "render/material/material_logic.h"
#include "render/unit/unit_comp.h"
#include "render/light/light_comp.h"
#include "context.h"
#include "engine_object.h"
#include "editor/test.h"

namespace Editor
{
	void Test::CreateMainCamera(Context *context)
	{
		auto mainCameraEO = std::make_shared<EngineObject>();
		mainCameraEO->name = Name_MainCamera;

		Logic::TransformLogic::Add(mainCameraEO,
								   glm::vec3(0.0f, 3.8f, -10.0f),
								   glm::vec3(15.0f, 0.0f, 0.0f),
								   glm::vec3(1.0f, 1.0f, 1.0f));

		auto mainCamera = std::make_shared<Logic::Camera>();
		mainCamera->near = 0.1f;
		mainCamera->far = 100.0f;
		mainCamera->mode = Logic::CameraMode::ePerspective;
		mainCamera->fov = 50.0f;
		mainCamera->size = 15.0f;
		mainCamera->passName = Render::Pass_Main;
		mainCameraEO->AddComponent<Logic::Camera>(mainCamera);

		context->logicMainCameraEO = mainCameraEO;
		context->AddEO(mainCameraEO);
	}

	void Test::CreateLight(Context *context)
	{
		auto directionLightEO = std::make_shared<EngineObject>();
		directionLightEO->name = Name_DirectionLight;

		Logic::TransformLogic::Add(directionLightEO,
								   glm::vec3(-5.0f, 5.0f, 0.0f),
								   glm::vec3(45.0f, 90.0f, 0.0f),
								   glm::vec3(1.0f, 1.0f, 1.0f));

		auto directionLight = std::make_shared<Render::DirectionLight>();
		directionLight->color = glm::vec3(1.0f, 0.9568627f, 0.8392157f);
		directionLight->params = glm::vec4(0.4f, 10.0f, 0.5f, 0.0f);
		directionLight->hasShadow = true;

		directionLightEO->AddComponent(directionLight);

		auto directionLightCamera = std::make_shared<Logic::Camera>();
		directionLightCamera->near = 0.1f;
		directionLightCamera->far = 50.0f;
		directionLightCamera->mode = Logic::CameraMode::eOrtho;
		directionLightCamera->size = 50.0f;
		directionLightCamera->passName = Render::Pass_Shadow;
		directionLightEO->AddComponent<Logic::Camera>(directionLightCamera);

		auto directionLightUnit = std::make_shared<Render::Unit>();

		context->AddEO(directionLightEO);
	}

	void Test::CreateSkybox(Context *context)
	{
		auto skyboxEO = std::make_shared<EngineObject>();
		skyboxEO->name = Name_Skybox;

		Logic::TransformLogic::Add(skyboxEO,
								   glm::vec3(0.0f, 0.0f, 0.0f),
								   glm::vec3(0.0f, -100.0f, 0.0f),
								   glm::vec3(50.0f, 50.0f, 50.0f));

		auto skyboxMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/base/sphere.obj");
		auto skyboxMaterial = Render::MaterialLogic::CreateByImageCube(context, Render::Pipeline_Skybox,
																	   {"resource/texture/skybox2/sky_left_lnitial.png",
																		"resource/texture/skybox2/sky_right_lnitial.png",
																		"resource/texture/skybox2/sky_up_lnitial.png",
																		"resource/texture/skybox2/sky_down_lnitial.png",
																		"resource/texture/skybox2/sky_front_lnitial.png",
																		"resource/texture/skybox2/sky_back_lnitial.png"});
		auto skyboxUnit = std::make_shared<Render::Unit>();

		skyboxEO->AddComponent(skyboxMesh);
		skyboxEO->AddComponent(skyboxMaterial);
		skyboxEO->AddComponent(skyboxUnit);

		context->AddEO(skyboxEO);
	}

	void Test::CreateModel(Context *context)
	{
		auto modelEO = std::make_shared<EngineObject>();
		modelEO->name = "Model-VikingRoom";

		Logic::TransformLogic::Add(modelEO,
								   glm::vec3(0.0f, -0.5f, 0.0f),
								   glm::vec3(-90.0f, -45.0f, 0.0f),
								   glm::vec3(4.0f, 4.0f, 4.0f));

		auto modelMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/viking_room/viking_room.obj");
		auto modelMaterial = Render::MaterialLogic::CreateByImage(context, Render::Pipeline_Bling_Phone, "resource/obj/viking_room/viking_room.png");
		modelMaterial->castShadow = true;
		auto modelUnit = std::make_shared<Render::Unit>();

		modelEO->AddComponent(modelMesh);
		modelEO->AddComponent(modelMaterial);
		modelEO->AddComponent(modelUnit);

		context->AddEO(modelEO);
	}

	void Test::CreateAxis(Context *context)
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
			modelEO->name = "X";
			modelEO->hideInHierarchy = true;

			Logic::TransformLogic::Add(modelEO,
									   glm::vec3(1.0f, 0.0f, 0.0f),
									   glm::vec3(0.0f, 90.0f, 0.0f),
									   glm::vec3(1.0f, 1.0f, 1.0f));
			Logic::TransformLogic::SetParent(modelEO, modelParentEO);

			auto modelMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/base/axis.obj", glm::vec3(1.0f, 0.0f, 0.0f));
			auto modelMaterial = Render::MaterialLogic::CreateByImage(context, Render::Pipeline_Color, "resource/obj/viking_room/viking_room.png");
			auto modelUnit = std::make_shared<Render::Unit>();

			modelEO->AddComponent(modelMesh);
			modelEO->AddComponent(modelMaterial);
			modelEO->AddComponent(modelUnit);

			context->AddEO(modelEO);
		}

		{
			auto modelEO = std::make_shared<EngineObject>();
			modelEO->name = "Y";
			modelEO->hideInHierarchy = true;

			Logic::TransformLogic::Add(modelEO,
									   glm::vec3(0.0f, 1.0f, 0.0f),
									   glm::vec3(-90.0f, 0.0f, 0.0f),
									   glm::vec3(1.0f, 1.0f, 1.0f));
			Logic::TransformLogic::SetParent(modelEO, modelParentEO);

			auto modelMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/base/axis.obj", glm::vec3(0.0f, 1.0f, 0.0f));
			auto modelMaterial = Render::MaterialLogic::CreateByImage(context, Render::Pipeline_Color, "resource/obj/viking_room/viking_room.png");
			auto modelUnit = std::make_shared<Render::Unit>();

			modelEO->AddComponent(modelMesh);
			modelEO->AddComponent(modelMaterial);
			modelEO->AddComponent(modelUnit);

			context->AddEO(modelEO);
		}

		{
			auto modelEO = std::make_shared<EngineObject>();
			modelEO->name = "Z";
			modelEO->hideInHierarchy = true;

			Logic::TransformLogic::Add(modelEO,
									   glm::vec3(0.0f, 0.0f, 1.0f),
									   glm::vec3(0.0f, 0.0f, 0.0f),
									   glm::vec3(1.0f, 1.0f, 1.0f));
			Logic::TransformLogic::SetParent(modelEO, modelParentEO);

			auto modelMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/base/axis.obj", glm::vec3(0.0f, 0.0f, 1.0f));
			auto modelMaterial = Render::MaterialLogic::CreateByImage(context, Render::Pipeline_Color, "resource/obj/viking_room/viking_room.png");
			auto modelUnit = std::make_shared<Render::Unit>();

			modelEO->AddComponent(modelMesh);
			modelEO->AddComponent(modelMaterial);
			modelEO->AddComponent(modelUnit);

			context->AddEO(modelEO);
		}
	}
}