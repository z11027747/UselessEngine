#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

class EngineObject;

namespace Logic
{
	struct MoveFowrard final
	{
		inline static std::string type{"Logic::MoveFowrard"};

		glm::vec3 dir;
	};

	struct MoveFollow final
	{
		inline static std::string type{"Logic::MoveFollow"};

		std::shared_ptr<EngineObject> targetEO;
		glm::vec3 offset;
	};
}
