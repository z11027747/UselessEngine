#pragma once

#include <glm/glm.hpp>

#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_system.h"
#include "context.h"
#include "base.h"

using namespace Logic;

namespace Editor {

	class Entry final {
	public:

		static void Test(Context* context) {

			auto& cameraEO = context->cameraEO;
			cameraEO = std::make_shared<EngineObject>();

			TransformSystem::Add(cameraEO,
				glm::vec3(0.0f, 0.0f, -10.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			);

			auto camera = std::make_shared<Camera>();
			cameraEO->AddComponent(camera);

		}

	};

}
