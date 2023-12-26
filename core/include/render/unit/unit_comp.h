#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "render/vk/buffer/buffer_comp.h"

namespace Render {

	struct Vertex final {
		glm::vec3 positionOS;
		glm::vec3 color;
		glm::vec2 uv0;
	};

	struct Unit final {

		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;

		std::shared_ptr<Buffer> vertexBuffer;
		std::shared_ptr<Buffer> indexBuffer;
	};

}
