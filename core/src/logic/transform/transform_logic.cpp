#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "base.h"

namespace Logic {

	void TransformLogic::Add(std::shared_ptr<EngineObject> eo,
		glm::vec3 p, glm::vec3 ea, glm::vec3 s) {

		auto transform = std::make_shared<Transform>();
		transform->position = p;
		transform->eulerAngles = ea;
		transform->scale = s;

		eo->AddComponent<Transform>(transform);
	}
}