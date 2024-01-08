#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "render/vk/buffer/buffer_comp.h"
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
            const float epsilon = 1e-6f; // Adjust the epsilon based on your precision requirements
            return glm::all(glm::epsilonEqual(positionOS, other.positionOS, epsilon)) &&
                   glm::all(glm::epsilonEqual(normalOS, other.normalOS, epsilon)) &&
                   glm::all(glm::epsilonEqual(color, other.color, epsilon)) &&
                   glm::all(glm::epsilonEqual(uv0, other.uv0, epsilon));
        }
    };

    struct Mesh final
    {
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;
    };

    struct Material final
    {
		std::string pipelineName;
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
            return (hash<glm::vec3>()(vertex.positionOS)) ^
                   (hash<glm::vec3>()(vertex.normalOS) << 1) ^
                   (hash<glm::vec3>()(vertex.color) << 2) ^
                   (hash<glm::vec2>()(vertex.uv0) << 3);
        }
    };
}