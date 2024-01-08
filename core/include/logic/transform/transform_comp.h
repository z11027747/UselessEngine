#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class EngineObject;

namespace Logic
{
	struct Transform final
	{
		glm::vec3 position;
		glm::vec3 eulerAngles;
		glm::vec3 scale;

		// calc
		glm::mat4 model;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 forward;

		std::shared_ptr<EngineObject> parentEO;
		std::vector<std::shared_ptr<EngineObject>> childEOs;
	};
}
