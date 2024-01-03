#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "logic/transform/transform_comp.h"

class EngineObject;

namespace Logic
{
	class TransformLogic final
	{
	public:
		static void Add(std::shared_ptr<EngineObject> eo,
						glm::vec3, glm::vec3, glm::vec3);

		static glm::mat4 GetModel(std::shared_ptr<EngineObject> eo);
	};
}