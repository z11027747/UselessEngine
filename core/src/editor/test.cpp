
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_system.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_system.h"
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

		auto& cameraEO = context->cameraEO;
		cameraEO = std::make_shared<EngineObject>();

		Logic::TransformSystem::Add(cameraEO,
			glm::vec3(0.0f, 0.0f, -10.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		);

		Logic::CameraSystem::Add(cameraEO, {
			0.1f,
			100.0f,
			800.0f / 600.0f,//TODO
			45.0f
			});

		//  Cube =============================

		auto cubeEO = std::make_shared<EngineObject>();

		Logic::TransformSystem::Add(cubeEO,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		);

		auto cubeUnit = std::make_shared<Render::Unit>();

		std::vector<Render::Vertex> vertices;
		std::vector<uint16_t> indices;
		MakeTriangle(vertices, indices);

		Render::UnitLogic::SetVertices(context, cubeUnit, vertices);
		Render::UnitLogic::SetIndices(context, cubeUnit, indices);
		Render::UnitLogic::UpdateBuffers(context, cubeUnit);

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
			{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
		};

		indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			0, 3, 7, 7, 4, 0,
			1, 2, 6, 6, 5, 1,
			0, 1, 5, 5, 4, 0,
			2, 3, 7, 7, 6, 2
		};
	}

}