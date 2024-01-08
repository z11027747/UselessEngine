
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
		mainCameraEO->name = G_MainCamera;

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
		directionLightEO->name = "DirectionLight";

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

		Render::UnitLogic::SetPipelineName(context, directionLightUnit, "color");
		Render::UnitLogic::SetObj(context, directionLightUnit,
								  "resource/model/ef_wt_001_az5.obj", directionLight->color);

		directionLightEO->AddComponent<Render::Unit>(directionLightUnit);

		context->AddEO(directionLightEO);
	}

	void Test::CreateSkybox(Context *context)
	{
		auto skyboxEO = std::make_shared<EngineObject>();
		skyboxEO->name = "Skybox";

		Logic::TransformLogic::Add(skyboxEO,
								   glm::vec3(0.0f, 0.0f, 0.0f),
								   glm::vec3(0.0f, -100.0f, 0.0f),
								   glm::vec3(20.0f, 20.0f, 20.0f));

		auto skyboxUnit = std::make_shared<Render::Unit>();

		std::vector<Render::Vertex> vertices;
		std::vector<uint16_t> indices;
		MakeSkybox(vertices, indices);

		Render::UnitLogic::SetPipelineName(context, skyboxUnit, "skybox");
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

		Render::UnitLogic::SetPipelineName(context, modelUnit, "bling_phone");
		Render::UnitLogic::SetObj(context, modelUnit, "resource/model/viking_room/viking_room.obj");
		Render::UnitLogic::SetImage(context, modelUnit, "resource/model/viking_room/viking_room.png");

		modelEO->AddComponent<Render::Unit>(modelUnit);

		context->AddEO(modelEO);
	}

	void Test::MakeCube(
		std::vector<Render::Vertex> &vertices,
		std::vector<uint16_t> &indices)
	{
		vertices = {
			{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}};

		indices = {
			0, 2, 3,
			0, 3, 1,
			8, 4, 5,
			8, 5, 9,
			10, 6, 7,
			10, 7, 11,
			12, 13, 14,
			12, 14, 15,
			16, 17, 18,
			16, 18, 19,
			20, 21, 22,
			20, 22, 23};
	}

	void Test::MakeSkybox(
		std::vector<Render::Vertex> &vertices,
		std::vector<uint16_t> &indices)
	{
		vertices = {
			Render::Vertex{{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			Render::Vertex{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}};

		indices = {
			0, 1, 2,
			2, 3, 0,
			4, 1, 0,
			0, 5, 4,
			6, 7, 8,
			8, 9, 6,
			4, 5, 10,
			10, 11, 4,
			0, 3, 10,
			10, 5, 0,
			1, 4, 2,
			2, 4, 11};
	}
}