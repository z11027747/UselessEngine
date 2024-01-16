
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "common/res_system.h"
#include "engine_object.h"
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
    void MeshInstanceLogic::CreateCache(Context *context)
    {
        auto &cacheEO = context->renderCacheEo;

        auto instanceCache = std::make_shared<MeshInstanceCache>();
        cacheEO->AddComponent(instanceCache);
    }

    void MeshInstanceLogic::DestroyCache(Context *context)
    {
        auto &cacheEO = context->renderCacheEo;
        auto instanceCache = cacheEO->GetComponent<MeshInstanceCache>();

        auto &sharedMap = instanceCache->sharedMap;
        for (const auto &kv : sharedMap)
        {
            auto &sharedInstance = kv.second;
            Destroy(context, sharedInstance);
        }
        sharedMap.clear();

        // TODO 没用的应该及时删除
        auto &deleteVector = instanceCache->deleteVector;
        for (const auto &instance : deleteVector)
        {
            Destroy(context, instance);
        }
        deleteVector.clear();
    }

    std::shared_ptr<MeshInstance> MeshInstanceLogic::Get(Context *context,
                                                         const std::string &objName)
    {
        auto &cacheEO = context->renderCacheEo;
        auto instanceCache = cacheEO->GetComponent<MeshInstanceCache>();

        auto &sharedMap = instanceCache->sharedMap;
        if (sharedMap.find(objName) == sharedMap.end())
        {
            sharedMap[objName] = Create(context, objName);
        }

        return sharedMap[objName];
    }

    std::shared_ptr<MeshInstance> MeshInstanceLogic::Create(Context *context,
                                                            const std::string &objName, const glm::vec3 &vertexColor)
    {
        auto instance = std::make_shared<MeshInstance>();
        instance->objName = objName;

        LoadObj(context,
                instance,
                objName, vertexColor);
        CalcBoundingSphere(context, instance);
        CreateBuffer(context, instance);

        return instance;
    }

    void MeshInstanceLogic::Destroy(Context *context,
                                    std::shared_ptr<MeshInstance> instance)
    {
        BufferLogic::Destroy(context, instance->vertexBuffer);
        BufferLogic::Destroy(context, instance->indexBuffer);
    }

    void MeshInstanceLogic::SetDestroy(Context *context,
                                       std::shared_ptr<MeshInstance> instance)
    {
        auto &cacheEO = context->renderCacheEo;
        auto instanceCache = cacheEO->GetComponent<MeshInstanceCache>();

        instanceCache->deleteVector.push_back(instance);
    }

    void MeshInstanceLogic::LoadObj(Context *context,
                                    std::shared_ptr<MeshInstance> instance,
                                    const std::string &objName, const glm::vec3 &vertexColor)
    {
        auto &resObj = Common::ResSystem::LoadObj(objName);
        auto &attrib = resObj.attrib;
        auto &shapes = resObj.shapes;

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
                vertex.positionOS = {attrib.vertices[3 * vi + 0], attrib.vertices[3 * vi + 1], attrib.vertices[3 * vi + 2]};
                vertex.positionOS.x *= -1;

                if (attrib.normals.size() > 0)
                {
                    vertex.normalOS = {attrib.normals[3 * ni + 0], attrib.normals[3 * ni + 1], attrib.normals[3 * ni + 2]};
                    vertex.normalOS.x *= -1;
                }
                else
                {
                    vertex.normalOS = {0.0f, 0.0f, 0.0f};
                }

                vertex.color = vertexColor;

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

        instance->vertices = vertices;
        instance->indices = indices;
    }

    void MeshInstanceLogic::CalcBoundingSphere(Context *context,
                                               std::shared_ptr<MeshInstance> instance)
    {
        glm::vec3 minPos = glm::vec3(99999.0f);
        glm::vec3 maxPos = glm::vec3(-99999.0f);

        auto &vertices = instance->vertices;
        for (const auto &vertex : vertices)
        {
            auto &position = vertex.positionOS;

            if (minPos.x > position.x)
                minPos.x = position.x;
            if (minPos.y > position.y)
                minPos.y = position.y;
            if (minPos.z > position.z)
                minPos.z = position.z;

            if (maxPos.x < position.x)
                maxPos.x = position.x;
            if (maxPos.y < position.y)
                maxPos.y = position.y;
            if (maxPos.z < position.z)
                maxPos.z = position.z;
        }

        BoundingSphere boundingSphere = {};
        boundingSphere.center = (minPos + maxPos) * 0.5f;
        boundingSphere.radius = glm::distance(minPos, maxPos) * 0.5f * 0.8f;

        instance->boundingSphere = boundingSphere;
    }

    void MeshInstanceLogic::CreateBuffer(Context *context,
                                         std::shared_ptr<MeshInstance> instance)
    {
        auto &vertices = instance->vertices;
        auto &indices = instance->indices;

        auto vertexSize = static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size());
        auto indexSize = static_cast<VkDeviceSize>(sizeof(uint16_t) * indices.size());

        auto &vertexBuffer = instance->vertexBuffer;
        auto &indexBuffer = instance->indexBuffer;

        vertexBuffer = BufferLogic::Create(context,
                                           vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        indexBuffer = BufferLogic::Create(context,
                                          indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        BufferSetLogic::SetVector(context,
                                  vertexBuffer,
                                  vertices);
        BufferSetLogic::SetVector(context,
                                  indexBuffer,
                                  indices);
    }
}