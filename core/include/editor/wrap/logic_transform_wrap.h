#pragma once

#include <string>
#include <memory>
#include "logic/transform/transform_comp.h"

class Context;

namespace Editor {

	class LogicTransformWrap {
	public:

		static void Draw(Context*,
			std::shared_ptr<Logic::Transform>);

	};

}