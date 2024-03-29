#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include "logic/transform/transform_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
	void TransformLogic::Add(std::shared_ptr<EngineObject> eo,
							 const glm::vec3 &p, const glm::vec3 &ea)
	{
		auto transform = std::make_shared<Transform>();
		transform->localPosition = p;
		transform->localEulerAngles = ea;
		transform->localScale = glm::vec3(1.0f);

		eo->_AddComponent<Transform>(transform);
	}
	void TransformLogic::Add(std::shared_ptr<EngineObject> eo,
							 const glm::vec3 &p, const glm::vec3 &ea, const glm::vec3 &s)
	{
		auto transform = std::make_shared<Transform>();
		transform->localPosition = p;
		transform->localEulerAngles = ea;
		transform->localScale = s;

		eo->_AddComponent<Transform>(transform);
	}

	void TransformLogic::SetScale(std::shared_ptr<EngineObject> eo, const glm::vec3 &s)
	{
		auto transform = eo->GetComponent<Logic::Transform>();
		transform->localScale = s;
	}

	void TransformLogic::SetScale(std::shared_ptr<EngineObject> eo, float s)
	{
		auto transform = eo->GetComponent<Logic::Transform>();
		transform->localScale = glm::vec3(s);
	}

	void TransformLogic::ResetAll(std::shared_ptr<EngineObject> eo)
	{
		auto transform = eo->GetComponent<Logic::Transform>();
		transform->localPosition = glm::vec3(0.0f);
		transform->localEulerAngles = glm::vec3(0.0f);
		transform->localScale = glm::vec3(1.0f);
	}

	void TransformLogic::UpdateModel(Context *context, std::shared_ptr<EngineObject> eo)
	{
		auto transform = eo->GetComponent<Logic::Transform>();

		auto translation = glm::translate(glm::mat4(1.0f), transform->localPosition);
		auto localEulerAngles_rad = glm::radians(transform->localEulerAngles);
		auto rotation = glm::yawPitchRoll(localEulerAngles_rad.y, localEulerAngles_rad.x, localEulerAngles_rad.z);
		// auto rotation = glm::toMat4(glm::quat(glm::radians(transform->localEulerAngles)));
		auto scale = glm::scale(glm::mat4(1.0f), transform->localScale);

		auto model = translation * rotation * scale;

		auto &parentEOName = transform->parentEOName;
		if (!parentEOName.empty())
		{
			auto &parentEO = context->GetEO(parentEOName);
			auto parentTransform = parentEO->GetComponent<Logic::Transform>();
			model = parentTransform->model * model;

			transform->worldPosition = transform->localPosition + parentTransform->localPosition;
			transform->worldScale = transform->localScale * parentTransform->localScale;
		}
		else
		{
			transform->worldPosition = transform->localPosition;
			transform->worldScale = transform->localScale;
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
		childTransform->parentEOName = parentEO->name;

		auto parentTransform = parentEO->GetComponent<Logic::Transform>();
		parentTransform->childEONames.push_back(childEO->name);
	}

	float TransformLogic::Distance(std::shared_ptr<EngineObject> aEO, std::shared_ptr<EngineObject> bEO)
	{
		auto aTransform = aEO->GetComponent<Logic::Transform>();
		auto bTransform = bEO->GetComponent<Logic::Transform>();

		auto &aPosition = aTransform->worldPosition;
		auto &bPosition = bTransform->worldPosition;

		auto distance = glm::distance(aPosition, bPosition);
		return distance;
	}

}