#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
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
		alignas(8) glm::vec2 uv0;

		bool operator==(const Vertex &other) const
		{
			return positionOS == other.positionOS && normalOS == other.normalOS && color == other.color && uv0 == other.uv0;
		}
	};

	struct Unit final
	{
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
	};
}

namespace std
{
	template <>
	struct hash<Render::Vertex>
	{
		size_t operator()(Render::Vertex const &vertex) const
		{
			return (((((hash<glm::vec3>()(vertex.positionOS) 
           	 	^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) 
           		 ^ (hash<glm::vec2>()(vertex.uv0) << 1)) 
           		 ^ (hash<glm::vec3>()(vertex.normalOS) << 1)));
		}
	};
}