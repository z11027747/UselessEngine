
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "common/res_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace std
{
    template <>
    struct hash<Render::Vertex>
    {
        size_t operator()(Render::Vertex const &vertex) const
        {
            return hash<glm::vec3>()(vertex.positionOS) ^
                   (hash<glm::vec3>()(vertex.normalOS) << 1) ^
                   ((hash<glm::vec3>()(vertex.tangentOS) << 1) >> 1) ^
                   ((hash<glm::vec3>()(vertex.color) << 2) >> 1) ^
                   ((hash<glm::vec2>()(vertex.uv0) << 3) >> 1);
        }
    };
}

namespace Render
{
    void MeshInstanceLogic::CreateCache(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;

        auto instanceCache = std::make_shared<MeshInstanceCache>();
        context->AddComponent(globalEO, instanceCache);
    }

    void MeshInstanceLogic::DestroyCache(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MeshInstanceCache>();

        auto &sharedMap = instanceCache->sharedMap;
        for (const auto &kv : sharedMap)
        {
            auto &sharedInstance = kv.second;
            Destroy(context, sharedInstance);
        }
        sharedMap.clear();

        // TODO 没用的应该及时删除
        auto &deletes = instanceCache->deletes;
        for (const auto &instance : deletes)
        {
            Destroy(context, instance);
        }
        deletes.clear();

        context->RemoveComponent<MeshInstanceCache>(globalEO);
    }

    std::shared_ptr<MeshInstance> MeshInstanceLogic::Get(Context *context,
                                                         std::shared_ptr<MeshInfo> info)
    {
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MeshInstanceCache>();

        auto &objName = info->objName;
        auto &sharedMap = instanceCache->sharedMap;
        if (sharedMap.find(objName) == sharedMap.end())
        {
            sharedMap[objName] = Create(context, info);
        }

        return sharedMap[objName];
    }

    static int Id_Mesh = 0;

    std::shared_ptr<MeshInstance> MeshInstanceLogic::Create(Context *context,
                                                            std::shared_ptr<MeshInfo> info)
    {
        auto instance = std::make_shared<MeshInstance>();
        instance->id = Id_Mesh++;
        instance->info = info;

        LoadObj(context, instance);
        CreateBuffer(context, instance);
        CalcBoundingSphere(context, instance);

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
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MeshInstanceCache>();

        instanceCache->deletes.push_back(instance);
    }

    void MeshInstanceLogic::LoadObj(Context *context,
                                    std::shared_ptr<MeshInstance> instance)
    {
        auto &info = instance->info;

        auto &resObj = Common::ResSystem::LoadObj(info->objName);
        auto &attrib = resObj.attrib;
        auto &shapes = resObj.shapes;

        auto hasNormal = false;

        std::vector<Render::Vertex> uniqueVertices;

        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                auto vi = index.vertex_index;   // v
                auto ni = index.normal_index;   // vn
                auto ui = index.texcoord_index; // vt

                Render::Vertex vertex = {};
                vertex.positionOS = {attrib.vertices[3 * vi + 0], attrib.vertices[3 * vi + 1], attrib.vertices[3 * vi + 2]};
                vertex.positionOS.x *= -1; // 适配unity
                if (attrib.normals.size() > 0)
                {
                    hasNormal = true;
                    vertex.normalOS = {attrib.normals[3 * ni + 0], attrib.normals[3 * ni + 1], attrib.normals[3 * ni + 2]};
                    vertex.normalOS.x *= -1;
                }
                vertex.color = info->vertexColor;
                if (attrib.texcoords.size() > 0)
                {
                    vertex.uv0 = {attrib.texcoords[2 * ui + 0], 1.0f - attrib.texcoords[2 * ui + 1]};
                }

                uniqueVertices.push_back(vertex);
            }
        }

        auto uniqueVertexSize = static_cast<uint32_t>(uniqueVertices.size());

        // calc tangent
        if (hasNormal)
        {
            for (auto i = 0u; i < uniqueVertexSize; i += 3)
            {
                auto &v1 = uniqueVertices[i + 0];
                auto &v2 = uniqueVertices[i + 1];
                auto &v3 = uniqueVertices[i + 2];

                auto deltaPos1 = v2.positionOS - v1.positionOS;
                auto deltaPos2 = v3.positionOS - v1.positionOS;
                auto deltaUV1 = v2.uv0 - v1.uv0;
                auto deltaUV2 = v3.uv0 - v1.uv0;

                auto det = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
                if (det != 0.0f)
                {
                    auto tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) / det;
                    v1.tangentOS = glm::normalize(tangent);
                    v2.tangentOS = v1.tangentOS;
                    v3.tangentOS = v1.tangentOS;
                }
            }
        }

        std::vector<Render::Vertex> vertices;
        std::vector<uint16_t> indices;
        std::unordered_map<Render::Vertex, uint16_t> uniqueVertexIndex = {};

        // TODO 水特殊处理逻辑 临时
        auto isWater = (instance->info->objName == "resource/obj/water/water_30x30.obj");
        if (isWater)
        {
            for (auto i = 0u; i < uniqueVertexSize; i++)
            {
                indices.push_back(vertices.size());
                vertices.push_back(uniqueVertices[i]);
            }
            std::vector<Render::Vertex> originalVertices = vertices;
            for (auto i = 0u; i < uniqueVertexSize; i += 6)
            {
                auto &v0 = originalVertices[i + 0]; // 0
                auto &v1 = originalVertices[i + 1]; // 1
                auto &v2 = originalVertices[i + 2]; // 2
                auto &v5 = originalVertices[i + 5]; // 3
                
                vertices[i+0].positionOS = v0.positionOS; vertices[i+0].normalOS = v1.positionOS; vertices[i+0].tangentOS = v2.positionOS;
                vertices[i+1].positionOS = v1.positionOS; vertices[i+1].normalOS = v2.positionOS; vertices[i+1].tangentOS = v0.positionOS;
                vertices[i+2].positionOS = v2.positionOS; vertices[i+2].normalOS = v0.positionOS; vertices[i+2].tangentOS = v1.positionOS;
                
                vertices[i+3].positionOS = v0.positionOS; vertices[i+3].normalOS = v2.positionOS; vertices[i+3].tangentOS = v5.positionOS;
                vertices[i+4].positionOS = v2.positionOS; vertices[i+4].normalOS = v5.positionOS; vertices[i+4].tangentOS = v0.positionOS;
                vertices[i+5].positionOS = v5.positionOS; vertices[i+5].normalOS = v0.positionOS; vertices[i+5].tangentOS = v2.positionOS;
            }
        }
        else
        {
            for (auto i = 0u; i < uniqueVertexSize; i++)
            {
                auto &vertex = uniqueVertices[i];
                if (uniqueVertexIndex.count(vertex) == 0)
                {
                    uniqueVertexIndex[vertex] = static_cast<uint16_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertexIndex[vertex]);
            }
        }

        instance->vertices = std::move(vertices);
        instance->indices = std::move(indices);
    }

    void MeshInstanceLogic::CalcBoundingSphere(Context *context,
                                               std::shared_ptr<MeshInstance> instance)
    {
        auto minPos = glm::vec3(99999.0f);
        auto maxPos = glm::vec3(-99999.0f);

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