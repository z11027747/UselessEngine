#pragma once

#include <string>
#include <memory>
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"

class Context;

namespace Editor {

	class LogicComponentWrap {
	public:

		static void DrawCamera(Context*, std::shared_ptr<Logic::Camera>);
		static void DrawTransform(Context*, std::shared_ptr<Logic::Transform>);

	};

}