#pragma once

#include <glm/glm.hpp>
#include <memory>

class EngineObject;

namespace Logic
{
	struct MoveFowrard final
	{
		glm::vec3 dir;
	};

	struct MoveFollow final
	{
		std::shared_ptr<EngineObject> targetEO;
		glm::vec3 offset;
	};
}
