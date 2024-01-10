
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>
#include "render/mesh/mesh_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "common/res_system.h"
#include "context.h"

namespace std
{
    template <>
    struct hash<Render::Vertex>
    {
        size_t operator()(Render::Vertex const &vertex) const
        {
            return hash<glm::vec3>()(vertex.positionOS) ^
                   hash<glm::vec3>()(vertex.normalOS) ^
                   hash<glm::vec3>()(vertex.color) ^
                   hash<glm::vec2>()(vertex.uv0);
        }
    };
}

namespace Render
{
    std::shared_ptr<Mesh> MeshLogic::CreateByObj(Context *context,
                                                 const std::string &name, glm::vec3 &defaultColor)
    {
        auto mesh = std::make_shared<Mesh>();
        LoadObj(context, mesh, name, defaultColor);
        CreateBuffer(context, mesh);
        CalcBoundingSphere(context, mesh);
        return mesh;
    }

    void MeshLogic::Destroy(Context *context,
                            std::shared_ptr<Mesh> mesh)
    {
        BufferLogic::Destroy(context, mesh->vertexBuffer);
        BufferLogic::Destroy(context, mesh->indexBuffer);
    }

    void MeshLogic::LoadObj(Context *context,
                            std::shared_ptr<Mesh> mesh,
                            const std::string &objName, glm::vec3 &defaultColor)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;

        Common::ResSystem::LoadObjShapes(objName, attrib, shapes);

        std::vector<Render::Vertex> vertices;
        std::vector<uint16_t> indices;
        std::unordered_map<Render::Vertex, uint16_t> uniqueVertices = {};

        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                auto vi = index.vertex_index;
                auto ni = index.normal_index;

                Render::Vertex vertex = {};
                vertex.positionOS = {-attrib.vertices[3 * vi + 0],
                                     attrib.vertices[3 * vi + 1],
                                     attrib.vertices[3 * vi + 2]};
                vertex.normalOS = {-attrib.normals[3 * ni + 0],
                                   attrib.normals[3 * ni + 1],
                                   attrib.normals[3 * ni + 2]};
                vertex.color = defaultColor;
                if (attrib.texcoords.size() > 0)
                {
                    auto ui = index.texcoord_index;
                    vertex.uv0 = {attrib.texcoords[2 * ui + 0], 1.0f - attrib.texcoords[2 * ui + 1]};
                }
                else
                {
                    vertex.uv0 = {0.0f, 0.0f};
                }

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint16_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        mesh->objName = objName;
        mesh->vertices = vertices;
        mesh->indices = indices;
    }

    void MeshLogic::CreateBuffer(Context *context,
                                 std::shared_ptr<Mesh> mesh)
    {
        auto &vertices = mesh->vertices;
        auto &indices = mesh->indices;

        auto vertexSize = static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size());
        auto indexSize = static_cast<VkDeviceSize>(sizeof(uint16_t) * indices.size());

        mesh->vertexBuffer = BufferLogic::Create(context,
                                                 vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        mesh->indexBuffer = BufferLogic::Create(context,
                                                indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        BufferSetLogic::SetVector(context,
                                  mesh->vertexBuffer,
                                  vertices);
        BufferSetLogic::SetVector(context,
                                  mesh->indexBuffer,
                                  indices);
    }

    void MeshLogic::CalcBoundingSphere(Context *context,
                                       std::shared_ptr<Mesh> mesh)
    {
        glm::vec3 sumPositionOS = glm::vec3(0.0f);
        float maxDistance = 0.0f;

        auto &vertices = mesh->vertices;
        for (const auto &vertex : vertices)
        {
            auto &positionOS = vertex.positionOS;
            auto distance = glm::length(positionOS);

            sumPositionOS += positionOS;
            maxDistance = std::max(distance, maxDistance);
        }

        BoundingSphere boundingSphere = {};
        boundingSphere.center = sumPositionOS / static_cast<float>(vertices.size());
        boundingSphere.radius = maxDistance;

        mesh->bound = boundingSphere;
    }
}