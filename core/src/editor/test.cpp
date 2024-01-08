
#include <vector>
#include <string>
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/unit/unit_comp.h"
#include "render/unit/unit_logic.h"
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
								   glm::vec3(20.0f, 20.0f, 20.0f));

		auto skyboxUnit = std::make_shared<Render::Unit>();

		std::vector<Render::Vertex> vertices;
		std::vector<uint16_t> indices;
		MakeSkybox(vertices, indices);

		Render::UnitLogic::SetPipelineName(context, skyboxUnit, Render::Pipeline_Skybox);
		Render::UnitLogic::SetVertices(context, skyboxUnit, vertices);
		Render::UnitLogic::SetIndices(context, skyboxUnit, indices);

		std::array<std::string, 6> names = {
			"resource/texture/skybox2/sky_left_lnitial.png",
			"resource/texture/skybox2/sky_right_lnitial.png",
			"resource/texture/skybox2/sky_up_lnitial.png",
			"resource/texture/skybox2/sky_down_lnitial.png",
			"resource/texture/skybox2/sky_front_lnitial.png",
			"resource/texture/skybox2/sky_back_lnitial.png"};
		Render::UnitLogic::SetImageCube(context, skyboxUnit, names);

		skyboxEO->AddComponent<Render::Unit>(skyboxUnit);

		context->AddEO(skyboxEO);
	}

	void Test::CreateCube(Context *context)
	{
		// Cube1
		{
			auto cubeEO = std::make_shared<EngineObject>();
			cubeEO->name = "Cube1";

			Logic::TransformLogic::Add(cubeEO,
									   glm::vec3(-7.0f, 0.0f, 5.0f),
									   glm::vec3(10.0f, 20.0f, 0.0f),
									   glm::vec3(1.0f, 1.0f, 1.0f));

			auto cubeUnit = std::make_shared<Render::Unit>();

			std::vector<Render::Vertex> vertices;
			std::vector<uint16_t> indices;
			MakeCube(vertices, indices);

			Render::UnitLogic::SetPipelineName(context, cubeUnit, Render::Pipeline_Bling_Phone);
			Render::UnitLogic::SetVertices(context, cubeUnit, vertices);
			Render::UnitLogic::SetIndices(context, cubeUnit, indices);
			Render::UnitLogic::SetImage(context, cubeUnit, "resource/texture/container2.png");

			cubeEO->AddComponent<Render::Unit>(cubeUnit);

			context->AddEO(cubeEO);
		}
		// Cube2
		{
			auto cubeEO = std::make_shared<EngineObject>();
			cubeEO->name = "Cube2";

			Logic::TransformLogic::Add(cubeEO,
									   glm::vec3(5.7f, 0.0f, 0.0f),
									   glm::vec3(0.0f, 0.0f, 0.0f),
									   glm::vec3(1.0f, 1.0f, 1.0f));

			auto cubeUnit = std::make_shared<Render::Unit>();

			std::vector<Render::Vertex> vertices;
			std::vector<uint16_t> indices;
			MakeCube(vertices, indices);

			Render::UnitLogic::SetPipelineName(context, cubeUnit, Render::Pipeline_Bling_Phone);
			Render::UnitLogic::SetVertices(context, cubeUnit, vertices);
			Render::UnitLogic::SetIndices(context, cubeUnit, indices);
			Render::UnitLogic::SetImage(context, cubeUnit, "resource/texture/Wall03_Diffuse.jpg");

			cubeEO->AddComponent<Render::Unit>(cubeUnit);

			context->AddEO(cubeEO);
		}
	}

	void Test::CreateModel(Context *context)
	{
		auto modelEO = std::make_shared<EngineObject>();
		modelEO->name = "Model-VikingRoom";

		Logic::TransformLogic::Add(modelEO,
								   glm::vec3(0.0f, -0.5f, 0.0f),
								   glm::vec3(-90.0f, -45.0f, 0.0f),
								   glm::vec3(4.0f, 4.0f, 4.0f));

		auto modelUnit = std::make_shared<Render::Unit>();
		modelUnit->castShadow = true;

		Render::UnitLogic::SetPipelineName(context, modelUnit, Render::Pipeline_Bling_Phone);
		Render::UnitLogic::SetObj(context, modelUnit, "resource/obj/viking_room/viking_room.obj");
		Render::UnitLogic::SetImage(context, modelUnit, "resource/obj/viking_room/viking_room.png");

		modelEO->AddComponent<Render::Unit>(modelUnit);

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

			auto modelUnit = std::make_shared<Render::Unit>();
			modelUnit->castShadow = false;

			Render::UnitLogic::SetPipelineName(context, modelUnit, Render::Pipeline_Color);
			Render::UnitLogic::SetObj(context, modelUnit,
									  "resource/obj/basic/axis.obj", glm::vec3(1.0f, 0.0f, 0.0f));
			Render::UnitLogic::SetImage(context, modelUnit, "resource/texture/white.png");

			modelEO->AddComponent<Render::Unit>(modelUnit);

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

			auto modelUnit = std::make_shared<Render::Unit>();
			modelUnit->castShadow = false;

			Render::UnitLogic::SetPipelineName(context, modelUnit, Render::Pipeline_Color);
			Render::UnitLogic::SetObj(context, modelUnit,
									  "resource/obj/basic/axis.obj", glm::vec3(0.0f, 1.0f, 0.0f));
			Render::UnitLogic::SetImage(context, modelUnit, "resource/texture/white.png");

			modelEO->AddComponent<Render::Unit>(modelUnit);

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

			auto modelUnit = std::make_shared<Render::Unit>();
			modelUnit->castShadow = false;

			Render::UnitLogic::SetPipelineName(context, modelUnit, Render::Pipeline_Color);
			Render::UnitLogic::SetObj(context, modelUnit,
									  "resource/obj/basic/axis.obj", glm::vec3(0.0f, 0.0f, 1.0f));
			Render::UnitLogic::SetImage(context, modelUnit, "resource/texture/white.png");

			modelEO->AddComponent<Render::Unit>(modelUnit);

			context->AddEO(modelEO);
		}
	}
}