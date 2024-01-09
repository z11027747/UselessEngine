#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
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

	void TransformLogic::UpdateModel(std::shared_ptr<EngineObject> eo)
	{
		auto transform = eo->GetComponent<Logic::Transform>();

		auto translation = glm::translate(glm::mat4(1.0f), transform->position);
		auto rotation = glm::toMat4(glm::quat(glm::radians(transform->eulerAngles)));
		auto scale = glm::scale(glm::mat4(1.0f), transform->scale);

		auto model = translation * rotation * scale;

		if (transform->parentEO != nullptr)
		{
			auto parentTransform = transform->parentEO->GetComponent<Logic::Transform>();
			model = parentTransform->model * model;
		}

		transform->model = model;
		transform->forward = rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		transform->right = rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		transform->up = rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	}

	void TransformLogic::SetParent(std::shared_ptr<EngineObject> childEO,
								   std::shared_ptr<EngineObject> parentEO)
	{
		auto childTransform = childEO->GetComponent<Logic::Transform>();
		childTransform->parentEO = parentEO;

		auto parentTransform = parentEO->GetComponent<Logic::Transform>();
		parentTransform->childEOs.emplace_back(childEO);
	}

}