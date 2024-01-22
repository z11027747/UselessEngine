#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "render/vk/buffer/buffer_comp.h"

namespace Render
{
    struct Vertex final
    {
        alignas(16) glm::vec3 positionOS;
        alignas(16) glm::vec3 normalOS;
        alignas(16) glm::vec3 tangentOS;
        alignas(16) glm::vec3 color;
        alignas(8) glm::vec2 uv0;

        bool operator==(const Vertex &other) const
        {
            return (positionOS == other.positionOS) &&
                   (normalOS == other.normalOS) &&
                   (tangentOS == other.tangentOS) &&
                   (color == other.color) &&
                   (uv0 == other.uv0);
        }
    };

    struct BoundingSphere final
    {
        glm::vec3 center;
        float radius;
    };

    struct MeshInstance final
    {
        int id;
        std::string objName;
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;

        BoundingSphere boundingSphere;
    };

    struct Mesh final
    {
        std::string objName;
        glm::vec3 vertexColor{1.0f, 1.0f, 1.0f};
        bool checkHit{false};

        std::shared_ptr<MeshInstance> instance;
        bool hasChanged;
    };

    struct MeshInstanceCache final
    {
        std::unordered_map<std::string, std::shared_ptr<MeshInstance>> sharedMap{};
        std::vector<std::shared_ptr<MeshInstance>> deletes{};
    };
}
