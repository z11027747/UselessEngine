#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <memory>

#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "engine_object.h"

namespace Logic {

	void TransformLogic::Add(std::shared_ptr<EngineObject> eo,
		glm::vec3 p, glm::vec3 ea, glm::vec3 s) {

		auto transform = std::make_shared<Transform>();
		transform->position = p;
		transform->eulerAngles = ea;
		transform->scale = s;

		eo->AddComponent<Transform>(transform);
	}

	glm::mat4 TransformLogic::GetModel(std::shared_ptr<EngineObject> eo) {
		auto transform = eo->GetComponent<Logic::Transform>();

		auto model = glm::mat4(1.0f);
		model = glm::translate(model, transform->position);
		model = glm::rotate(model, glm::radians(transform->eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform->eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(transform->eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, transform->scale);
		return model;
	}

}