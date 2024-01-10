#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class EngineObject;

namespace Logic
{
	struct Transform final
	{
		glm::vec3 localPosition;
		glm::vec3 localEulerAngles;
		glm::vec3 localScale;

		glm::mat4 model;

		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 forward;

		std::shared_ptr<EngineObject> parentEO;
		std::vector<std::shared_ptr<EngineObject>> childEOs;
		
		glm::vec3 worldPosition;
		glm::vec3 worldScale;
	};
}
