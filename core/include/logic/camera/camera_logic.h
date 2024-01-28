#pragma once

#include <memory>
#include "logic/camera/camera_comp.h"

class Context;
class EngineObject;

namespace Logic
{
	class CameraLogic final
	{
	public:
		static void OnAdd(Context *, std::shared_ptr<EngineObject>);
		static void OnRemove(Context *, std::shared_ptr<EngineObject>);

		static void UpdateView(std::shared_ptr<EngineObject>);
		static void UpdateProjection(Context *, std::shared_ptr<Camera>);

		static glm::vec3 TransformNdcToWorld(Context *, const glm::vec3 &);
	};
}