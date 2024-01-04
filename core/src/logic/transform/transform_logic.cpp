#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include "logic/transform/transform_comp.h"
#include "logic/transform/transform_logic.h"
#include "engine_object.h"

namespace Logic
{
	void TransformLogic::Add(std::shared_ptr<EngineObject> eo,
							 glm::vec3 p, glm::vec3 ea, glm::vec3 s)
	{
		auto transform = std::make_shared<Transform>();
		transform->position = p;
		transform->eulerAngles = ea;
		transform->scale = s;

		eo->AddComponent<Transform>(transform);
	}

	glm::mat4 TransformLogic::GetModel(std::shared_ptr<EngineObject> eo)
	{
		auto transform = eo->GetComponent<Logic::Transform>();

		auto translation = glm::translate(glm::mat4(1.0f), transform->position);
		auto quat = glm::quat(glm::radians(transform->eulerAngles));
		auto rotation = glm::toMat4(quat);
		auto scale = glm::scale(glm::mat4(1.0f), transform->scale);

		auto model = translation * rotation * scale;
		return model;
	}

}