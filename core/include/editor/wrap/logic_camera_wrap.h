#pragma once

#include <string>
#include <memory>
#include "logic/camera/camera_comp.h"

class Context;

namespace Editor {

	class LogicCameraWrap {
	public:

		static void Draw(Context*,
			std::shared_ptr<Logic::Camera>);

	};

}