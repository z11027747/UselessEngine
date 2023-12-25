
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_system.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_system.h"
#include "render/unit/unit_comp.h"
#include "context.h"
#include "base.h"
#include "editor_test.h"

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

		std::vector<glm::vec3> cubeVertices = {
			{-0.5f, -0.5f, -0.5f},
			{ 0.5f, -0.5f, -0.5f},
			{ 0.5f,  0.5f, -0.5f},
			{-0.5f,  0.5f, -0.5f},
			{-0.5f, -0.5f,  0.5f},
			{ 0.5f, -0.5f,  0.5f},
			{ 0.5f,  0.5f,  0.5f},
			{-0.5f,  0.5f,  0.5f}
		};
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
		glm::vec2 uv = { 0.0f, 0.0f };

		for (int i = 0; i < 8; ++i) {
			Render::Vertex vertex;
			vertex.positionOS = cubeVertices[i];
			vertex.color = color;
			vertex.uv0 = uv;

			cubeUnit->vertices.push_back(vertex);
		}

		std::vector<uint32_t> cubeIndices = {
			0, 1, 2, 2, 3, 0, // 前面
			4, 5, 6, 6, 7, 4, // 后面
			0, 3, 7, 7, 4, 0, // 左侧
			1, 2, 6, 6, 5, 1, // 右侧
			0, 1, 5, 5, 4, 0, // 底部
			2, 3, 7, 7, 6, 2  // 顶部
		};
		cubeUnit->indices = cubeIndices;

		cubeEO->AddComponent<Render::Unit>(cubeUnit);
		context->renderUnitEOs.push_back(cubeEO);
	}

}