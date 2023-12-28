#pragma once

#include <memory>
#include "logic/camera/camera_comp.h"

class EngineObject;

namespace Logic {

	class CameraLogic final {
	public:

		static void Add(std::shared_ptr<EngineObject>, Frustum);

		static void UpdateView(std::shared_ptr<EngineObject>);
		static void UpdateProjection(std::shared_ptr<Camera>);

	};

}