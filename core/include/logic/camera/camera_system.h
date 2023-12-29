#pragma once

#include <glm/glm.hpp>
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_comp.h"
#include "logic/camera/camera_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Logic {

	class CameraSystem final {
	public:

		static void Create(Context* context) {
			auto cameraEO = std::make_shared<EngineObject>();
			cameraEO->name = G_Camera;

			Logic::TransformLogic::Add(cameraEO,
				glm::vec3(0.0f, 5.0f, -14.0f),
				glm::vec3(12.8f, 0.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 1.0f)
			);

			Frustum frustum = {
				0.1f,
				50.0f,
				context->aspect,
				45.0f
			};
			Logic::CameraLogic::Add(cameraEO, frustum);

			context->AddEO(cameraEO);
		}

		static void UpdateView(Context* context) {

			auto cameraEO = context->GetEO(G_Camera);
			auto transform = cameraEO->GetComponent<Transform>();
			auto camera = cameraEO->GetComponent<Camera>();

			if (transform->position != camera->calcPos
				|| transform->eulerAngles != camera->calcEul) {

				CameraLogic::UpdateView(cameraEO);
			}

		}

	};

}
