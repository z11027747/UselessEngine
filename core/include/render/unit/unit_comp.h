#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"

namespace Render {

	struct Vertex final {
		glm::vec3 positionOS;
		glm::vec3 color;
		glm::vec2 uv0;
	};

	struct Unit final {
		std::string pipelineName;

		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;

		std::shared_ptr<Buffer> vertexBuffer;
		std::shared_ptr<Buffer> indexBuffer;

		std::shared_ptr<Descriptor> descriptor;
	};

}
