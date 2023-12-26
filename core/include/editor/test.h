#pragma once

#include <render/unit/unit_comp.h>

class Context;

namespace Editor {

	class Test final {
	public:

		static void Create(Context* context);

		static void MakeTriangle(
			std::vector<Render::Vertex>& vertices,
			std::vector<uint16_t>& indices);

		static void MakeCube(
			std::vector<Render::Vertex>& vertices,
			std::vector<uint16_t>& indices);
	};

}
