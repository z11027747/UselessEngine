
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
#include "context.h"
#include "engine_object.h"
#include "editor/test.h"

namespace Editor {

	void Test::CreateCube(Context* context) {

		{
			auto cubeEO = std::make_shared<EngineObject>();
			cubeEO->name = "Cube1";

			Logic::TransformLogic::Add(cubeEO,
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 50.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			);

			auto cubeUnit = std::make_shared<Render::Unit>();

			std::vector<Render::Vertex> vertices;
			std::vector<uint16_t> indices;
			MakeCube(vertices, indices);

			Render::UnitLogic::SetPipelineName(context, cubeUnit, "test");
			Render::UnitLogic::SetVertices(context, cubeUnit, vertices);
			Render::UnitLogic::SetIndices(context, cubeUnit, indices);
			Render::UnitLogic::SetImage(context, cubeUnit, "resource/texture/icon2.png");

			context->AddEO(cubeEO);

			cubeEO->AddComponent<Render::Unit>(cubeUnit);
			context->renderUnitEOs.push_back(cubeEO);
		}


		{
			auto cubeEO = std::make_shared<EngineObject>();
			cubeEO->name = "Cube2";

			Logic::TransformLogic::Add(cubeEO,
				glm::vec3(0.0f, -0.5f, 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(10.0f, 0.1f, 10.0f)
			);

			auto cubeUnit = std::make_shared<Render::Unit>();

			std::vector<Render::Vertex> vertices;
			std::vector<uint16_t> indices;
			MakeCube(vertices, indices);

			Render::UnitLogic::SetPipelineName(context, cubeUnit, "test");
			Render::UnitLogic::SetVertices(context, cubeUnit, vertices);
			Render::UnitLogic::SetIndices(context, cubeUnit, indices);
			Render::UnitLogic::SetImage(context, cubeUnit, "resource/texture/Wall03_Diffuse.jpg");

			context->AddEO(cubeEO);

			cubeEO->AddComponent<Render::Unit>(cubeUnit);
			context->renderUnitEOs.push_back(cubeEO);
		}

	}

	void Test::CreateSkybox(Context* context) {

		auto skyboxEO = std::make_shared<EngineObject>();
		skyboxEO->name = "Skybox";

		Logic::TransformLogic::Add(skyboxEO,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		);

		auto skyboxUnit = std::make_shared<Render::Unit>();

		std::vector<Render::Vertex> vertices;
		std::vector<uint16_t> indices;
		MakeSkybox(vertices, indices);

		Render::UnitLogic::SetPipelineName(context, skyboxUnit, "skybox");
		Render::UnitLogic::SetVertices(context, skyboxUnit, vertices);
		Render::UnitLogic::SetIndices(context, skyboxUnit, indices);

		std::array<std::string, 6> names = {
			"resource/texture/skybox/right.jpg",
			"resource/texture/skybox/left.jpg",
			"resource/texture/skybox/top.jpg",
			"resource/texture/skybox/bottom.jpg",
			"resource/texture/skybox/front.jpg",
			"resource/texture/skybox/back.jpg"
		};
		Render::UnitLogic::SetImageCube(context, skyboxUnit, names);

		context->AddEO(skyboxEO);

		skyboxEO->AddComponent<Render::Unit>(skyboxUnit);
		context->renderUnitEOs.push_back(skyboxEO);
	}

	void Test::MakeTriangle(
		std::vector<Render::Vertex>& vertices,
		std::vector<uint16_t>& indices) {

		vertices = {
			{{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f , 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
		};

		indices = {
			0, 1, 2
		};
	}

	void Test::MakeCube(
		std::vector<Render::Vertex>& vertices,
		std::vector<uint16_t>& indices) {

		vertices = {
			{{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}
		};

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
			20, 22, 23
		};

	}

	glm::vec3 cubemapVertices[36] =
	{
		
	};

	void Test::MakeSkybox(
		std::vector<Render::Vertex>& vertices,
		std::vector<uint16_t>& indices
	) {
		vertices = {
			{{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{ -1.0f, -1.0f, -1.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{ 1.0f, -1.0f, -1.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{ 1.0f,  1.0f, -1.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{ -1.0f, -1.0f,  1.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{ -1.0f,  1.0f,  1.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{ 1.0f, -1.0f, -1.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
			{{ 1.0f, -1.0f,  1.0f } , {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{{ 1.0f,  1.0f,  1.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 1.0f,  1.0f, -1.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
			{{ 1.0f,  1.0f,  1.0f } , {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 1.0f, -1.0f,  1.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		};

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
			2, 4, 11
		};

	}
}