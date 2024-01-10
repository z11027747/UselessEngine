#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "render/vk/buffer/buffer_comp.h"

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
            return (positionOS == other.positionOS) &&
                   (normalOS == other.normalOS) &&
                   (color == other.color) &&
                   (uv0 == other.uv0);
        }
    };

    struct BoundingSphere final
    {
        glm::vec3 center;
        float radius;
    };

    struct Mesh final
    {
        std::string objName;
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;

        BoundingSphere bound;
    };
}
