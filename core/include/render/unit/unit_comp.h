#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Render {

	struct Vertex final {
		glm::vec3 positionOS;
		glm::vec3 color;
		glm::vec2 uv0;
	};

	struct Unit final {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

}
