#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "logic/transform/transform_comp.h"

class Context;
class EngineObject;

namespace Logic
{
	class TransformLogic final
	{
	public:
		static void Add(std::shared_ptr<EngineObject>,
						const glm::vec3 &, const glm::vec3 &);
		static void Add(std::shared_ptr<EngineObject>,
						const glm::vec3 &, const glm::vec3 &, const glm::vec3 &);
		static void SetScale(std::shared_ptr<EngineObject>, const glm::vec3 &);
		static void SetScale(std::shared_ptr<EngineObject>, float);
		static void ResetAll(std::shared_ptr<EngineObject>);

		static void UpdateModel(Context *, std::shared_ptr<EngineObject>);

		static void SetParent(std::shared_ptr<EngineObject>,
							  std::shared_ptr<EngineObject>);
	};
}