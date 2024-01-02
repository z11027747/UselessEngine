#pragma once

#include <string>
#include <memory>
#include "render/light/light_comp.h"

class Context;

namespace Editor {

	class RenderComponentWrap {
	public:

		static void DrawDirectionLight(Context*, std::shared_ptr<Render::DirectionLight>);

	};

}