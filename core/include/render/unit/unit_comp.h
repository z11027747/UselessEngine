#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"

namespace Render
{
	struct Unit final
	{
		// std::shared_ptr<Mesh> mesh;
		// std::shared_ptr<Material> matrial;

		// shader
		std::string pipelineName;

		// mesh
		std::string objName;
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
		std::shared_ptr<Buffer> vertexBuffer;
		std::shared_ptr<Buffer> indexBuffer;

		// image
		std::string textureName;
		std::shared_ptr<Image> image0;

		// descriptor
		std::shared_ptr<Descriptor> descriptor;

		bool castShadow;
	};
}
