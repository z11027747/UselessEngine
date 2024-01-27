#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>

namespace Logic
{
	struct Transform final
	{
		inline static std::string type{"Logic::Transform"};

		glm::vec3 localPosition;
		glm::vec3 localEulerAngles;
		glm::vec3 localScale;
		std::string parentEOName;
		std::vector<std::string> childEONames;

		glm::mat4 model;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 forward;
		glm::vec3 worldPosition;
		// glm::vec3 worldEulerAngles;
		glm::vec3 worldScale;
	};
}
