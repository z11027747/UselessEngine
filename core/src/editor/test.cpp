
#include <vector>
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/unit/unit_comp.h"
#include "render/unit/unit_logic.h"
#include "context.h"
#include "base.h"
#include "editor/test.h"

namespace Editor {

	void Test::Create(Context* context) {

		//  Camera =============================

		auto aspect = context->aspect;
		auto& cameraEO = context->cameraEO;
		cameraEO = std::make_shared<EngineObject>();

		Logic::TransformLogic::Add(cameraEO,
			glm::vec3(0.0f, 0.0f, -10.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		);

		Logic::CameraLogic::Add(cameraEO, {
			0.1f,
			100.0f,
			aspect,
			45.0f
			});

		//  Cube =============================

		auto cubeEO = std::make_shared<EngineObject>();

		Logic::TransformLogic::Add(cubeEO,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(-30.0f, 50.0f, -30.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		);

		auto cubeUnit = std::make_shared<Render::Unit>();
		cubeUnit->pipelineName = "test";

		std::vector<Render::Vertex> vertices;
		std::vector<uint16_t> indices;
		//MakeTriangle(vertices, indices);
		MakeCube(vertices, indices);

		Render::UnitLogic::SetVertices(context, cubeUnit, vertices);
		Render::UnitLogic::SetIndices(context, cubeUnit, indices);

		cubeEO->AddComponent<Render::Unit>(cubeUnit);
		context->renderUnitEOs.push_back(cubeEO);
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
			{{0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, -0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{0.5, -0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
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
}