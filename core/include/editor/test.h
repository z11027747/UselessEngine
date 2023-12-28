#pragma once

#include <render/unit/unit_comp.h>

class Context;

namespace Editor {

	class Test final {
	public:

		static void CreateCube(Context* );
		static void CreateSkybox(Context*);

		static void MakeTriangle(
			std::vector<Render::Vertex>& vertices,
			std::vector<uint16_t>& indices);

		static void MakeCube(
			std::vector<Render::Vertex>& vertices,
			std::vector<uint16_t>& indices);

		static void MakeSkybox(
			std::vector<Render::Vertex>& vertices,
			std::vector<uint16_t>& indices);
	};

}
