
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
	void Test::CreateCube(Context *context)
	{
		// Cube1
		{
			auto cubeEO = std::make_shared<EngineObject>();
			cubeEO->name = "Cube1";

			Logic::TransformLogic::Add(cubeEO,
									   glm::vec3(0.0f, 0.0f, 0.0f),
									   glm::vec3(10.0f, 20.0f, 0.0f),
									   glm::vec3(1.0f, 1.0f, 1.0f));

			auto cubeUnit = std::make_shared<Render::Unit>();

			std::vector<Render::Vertex> vertices;
			std::vector<uint16_t> indices;
			MakeCube(vertices, indices);

			Render::UnitLogic::SetPipelineName(context, cubeUnit, "bling_phone");
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
									   glm::vec3(0.0f, -0.5f, 0.0f),
									   glm::vec3(0.0f, 0.0f, 0.0f),
									   glm::vec3(10.0f, 0.1f, 10.0f));

			auto cubeUnit = std::make_shared<Render::Unit>();

			std::vector<Render::Vertex> vertices;
			std::vector<uint16_t> indices;
			MakeCube(vertices, indices);

			Render::UnitLogic::SetPipelineName(context, cubeUnit, "bling_phone");
			Render::UnitLogic::SetVertices(context, cubeUnit, vertices);
			Render::UnitLogic::SetIndices(context, cubeUnit, indices);
			Render::UnitLogic::SetImage(context, cubeUnit, "resource/texture/Wall03_Diffuse.jpg");

			cubeEO->AddComponent<Render::Unit>(cubeUnit);

			context->AddEO(cubeEO);
		}
	}

	void Test::CreateSkybox(Context *context)
	{
		auto skyboxEO = std::make_shared<EngineObject>();
		skyboxEO->name = "Skybox";

		Logic::TransformLogic::Add(skyboxEO,
								   glm::vec3(0.0f, 0.0f, 0.0f),
								   glm::vec3(0.0f, 0.0f, 0.0f),
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

	void Test::CreateLight(Context *context)
	{
		auto directionLightEO = std::make_shared<EngineObject>();
		directionLightEO->name = "DirectionLight";

		Logic::TransformLogic::Add(directionLightEO,
								   glm::vec3(5.0f, 5.0f, 0.0f),
								   glm::vec3(45.0f, -90.0f, 0.0f),
								   glm::vec3(1.0f, 1.0f, 1.0f));

		auto directionLight = std::make_shared<Render::DirectionLight>();
		directionLight->color = glm::vec3(1.0f, 0.9568627f, 0.8392157f);
		directionLight->params = glm::vec4(0.4f, 10.0f, 0.5f, 0.0f);

		directionLightEO->AddComponent(directionLight);

		auto directionLightCamera = std::make_shared<Logic::Camera>();
		directionLightCamera->near = 0.1f;
		directionLightCamera->far = 20.0f;
		directionLightCamera->mode = Logic::CameraMode::eOrtho;
		directionLightCamera->size = 15.0f;
		directionLightCamera->renderPass = context->renderShadowPass;
		directionLightEO->AddComponent<Logic::Camera>(directionLightCamera);

		Logic::CameraLogic::UpdateView(directionLightEO);
		Logic::CameraLogic::UpdateProjection(context, directionLightCamera);

		context->renderLightEOs.push_back(directionLightEO);
		context->AddEO(directionLightEO);
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