#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"

namespace Render
{
	struct Vertex final
	{
		alignas(16) glm::vec3 positionOS;
		alignas(16) glm::vec3 normalOS;
		alignas(16) glm::vec3 color;
		glm::vec2 uv0;
	};

	struct Unit final
	{
		// shader
		std::string pipelineName;

		// mesh
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;

		// memory
		std::shared_ptr<Buffer> vertexBuffer;
		std::shared_ptr<Buffer> indexBuffer;
		std::shared_ptr<Image> image0;

		// descriptor
		std::shared_ptr<Descriptor> descriptor;
	};
}
