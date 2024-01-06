#pragma once

#include <glm/glm.hpp>
#include <memory>

class EngineObject;

namespace Logic
{
	class TransformLogic final
	{
	public:
		static void Add(std::shared_ptr<EngineObject> eo,
						glm::vec3, glm::vec3, glm::vec3);

		static void UpdateModel(std::shared_ptr<EngineObject> eo);
	};
}