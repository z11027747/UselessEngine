
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "common/res_system.h"
#include "common/log.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace std
{
    template <>
    struct hash<Render::Vertex>
    {
        size_t operator()(Render::Vertex const &vertex) const
        {
            auto h1 = hash<glm::vec3>()(vertex.positionOS);
            auto h2 = hash<glm::vec3>()(vertex.normalOS);
            auto h3 = hash<glm::vec3>()(vertex.tangentOS);
            auto h4 = hash<glm::vec3>()(vertex.color);
            auto h5 = hash<glm::vec2>()(vertex.uv0);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
        }
    };
}

namespace Render
{
    bool MeshLogic::IsShared(Context *context,
                             std::shared_ptr<Mesh> mesh)
    {
        // TODO
        return (mesh->info->vertexColor == glm::vec3(1.0f));
    }

    void MeshLogic::TryDestroyEO(Context *context,
                                 std::shared_ptr<EngineObject> eo)
    {
        auto mesh = eo->GetComponent<Render::Mesh>();
        if (!IsShared(context, mesh))
        {
            MeshLogic::SetDestroy(context, mesh->data);
        }
    }

    void MeshLogic::SetDestroy(Context *context,
                               std::shared_ptr<MeshData> data)
    {
        auto &globalEO = context->renderGlobalEO;
        auto meshCache = globalEO->GetComponent<MeshCache>();

        meshCache->deletes.push_back(data);
    }

    void MeshLogic::Destroy(Context *context,
                            std::shared_ptr<MeshData> data)
    {
        BufferLogic::Destroy(context, data->vertexBuffer);
        BufferLogic::Destroy(context, data->indexBuffer);
    }

    std::shared_ptr<MeshData> MeshLogic::Get(Context *context,
                                             std::shared_ptr<MeshInfo> info)
    {
        auto &globalEO = context->renderGlobalEO;
        auto meshCache = globalEO->GetComponent<MeshCache>();
        auto &sharedMap = meshCache->sharedMap;

        auto &objName = info->objName;
        if (sharedMap.find(objName) == sharedMap.end())
        {
            sharedMap[objName] = Create(context, info);
        }

        return sharedMap[objName];
    }

    static int Id_Mesh = 0;

    std::shared_ptr<MeshData> MeshLogic::Create(Context *context,
                                                std::shared_ptr<MeshInfo> info)
    {
        auto data = std::make_shared<MeshData>();
        data->id = Id_Mesh++;
        data->info = info;

        // LoadObj_TinyObj(context, data);
        LoadObj_Assimp(context, data);

        CreateBuffer(context, data);
        CalcBoundingSphere(context, data);

        return data;
    }

    void MeshLogic::LoadObj_TinyObj(Context *context,
                                    std::shared_ptr<MeshData> data)
    {
        auto &resObj = Common::ResSystem::LoadObj(data->info->objName);
        auto &attrib = resObj.attrib;
        auto &shapes = resObj.shapes;

        std::vector<Render::Vertex> allVertices;

        for (const auto &shape : shapes)
        {
            Render::Vertex vertex3[3];

            auto hasNormal = false;

            auto indexSize = shape.mesh.indices.size();
            for (auto i = 0; i < indexSize; i += 3)
            {
                for (auto v = 0; v < 3; v++)
                {
                    auto &index = shape.mesh.indices[i + v];

                    auto vi = index.vertex_index;   // v
                    auto ni = index.normal_index;   // vn
                    auto ui = index.texcoord_index; // vt

                    Render::Vertex vertex = {};
                    vertex.positionOS = {attrib.vertices[3 * vi + 0], attrib.vertices[3 * vi + 1], attrib.vertices[3 * vi + 2]};
                    vertex.positionOS.x *= -1;
                    if (attrib.normals.size() > 0)
                    {
                        hasNormal = true;
                        vertex.normalOS = {attrib.normals[3 * ni + 0], attrib.normals[3 * ni + 1], attrib.normals[3 * ni + 2]};
                        vertex.normalOS.x *= -1;
                    }
                    vertex.color = data->info->vertexColor;
                    if (attrib.texcoords.size() > 0)
                    {
                        vertex.uv0 = {attrib.texcoords[2 * ui + 0], 1.0f - attrib.texcoords[2 * ui + 1]};
                    }

                    vertex3[v] = vertex;
                }

                if (hasNormal)
                {
                    auto deltaPos1 = vertex3[1].positionOS - vertex3[0].positionOS;
                    auto deltaPos2 = vertex3[2].positionOS - vertex3[0].positionOS;
                    auto deltaUV1 = vertex3[1].uv0 - vertex3[0].uv0;
                    auto deltaUV2 = vertex3[2].uv0 - vertex3[0].uv0;

                    glm::vec3 tangentOS = {1.0f, 0.0f, 0.0f};
                    auto det = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
                    if (det != 0.0f)
                    {
                        tangentOS = glm::normalize((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) / det);
                    }
                    vertex3[0].tangentOS = tangentOS;
                    vertex3[1].tangentOS = tangentOS;
                    vertex3[2].tangentOS = tangentOS;
                }

                for (auto v = 0; v < 3; v++)
                {
                    allVertices.push_back(vertex3[v]);
                }
            }
        }

        LoadObj_VertexDeduplicated(context, data,
                                   allVertices);
    }

    static void LoadObj_Assimp_ProcessMesh(std::vector<Render::Vertex> &allVertices, glm::vec3 &vertexColor,
                                           aiMesh *mesh, const aiScene *scene)
    {
        for (auto faceIdx = 0; faceIdx < mesh->mNumFaces; ++faceIdx)
        {
            auto face = mesh->mFaces[faceIdx];
            for (auto indexIdx = 0; indexIdx < face.mNumIndices; ++indexIdx)
            {
                auto vertexIdx = face.mIndices[indexIdx];

                auto vertex = mesh->mVertices[vertexIdx];
                auto normal = mesh->mNormals[vertexIdx];
                aiVector3D tangent = {1.0f, 0.0f, 0.0f};
                if (mesh->HasTangentsAndBitangents())
                    tangent = mesh->mTangents[vertexIdx];
                aiVector3D textureCoords0 = {0.5f, 0.5f, 0.0f};
                if (mesh->HasTextureCoords(0))
                    textureCoords0 = mesh->mTextureCoords[0][vertexIdx];

                Render::Vertex v = {};
                v.positionOS = {vertex.x, vertex.y, vertex.z};
                v.positionOS.x *= -1;
                v.normalOS = {normal.x, normal.y, normal.z};
                v.normalOS.x *= -1;
                v.tangentOS = {tangent.x, tangent.y, tangent.z};
                v.color = vertexColor;
                v.uv0 = {textureCoords0.x, textureCoords0.y};
                allVertices.push_back(v);
            }
        }
    }

    static void LoadObj_Assimp_ProcessNode(std::vector<Render::Vertex> &allVertices, glm::vec3 &vertexColor,
                                           aiNode *aiNode, const aiScene *aiScene)
    {
        for (auto i = 0; i < aiNode->mNumMeshes; i++)
        {
            auto *aiMesh = aiScene->mMeshes[aiNode->mMeshes[i]];
            LoadObj_Assimp_ProcessMesh(allVertices, vertexColor,
                                       aiMesh, aiScene);
        }
        for (auto i = 0; i < aiNode->mNumChildren; i++)
        {
            LoadObj_Assimp_ProcessNode(allVertices, vertexColor,
                                       aiNode->mChildren[i], aiScene);
        }
    }

    void MeshLogic::LoadObj_Assimp(Context *context,
                                   std::shared_ptr<MeshData> data)
    {
        Assimp::Importer importer;

        auto aiProcess_Flags = aiProcess_Triangulate |
                               aiProcess_FlipUVs |
                               aiProcess_GenNormals | aiProcess_CalcTangentSpace;
        const auto *aiScene = importer.ReadFile(data->info->objName, aiProcess_Flags);

        if (!aiScene || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
        {
            Common::Log::Error("ERROR::ASSIMP::", importer.GetErrorString());
            return;
        }

        // 输出模型中顶点和面的数量
        std::cout << "Number of vertices: " << aiScene->mMeshes[0]->mNumVertices << std::endl;
        std::cout << "Number of faces: " << aiScene->mMeshes[0]->mNumFaces << std::endl;

        std::vector<Render::Vertex> allVertices;
        LoadObj_Assimp_ProcessNode(allVertices, data->info->vertexColor,
                                   aiScene->mRootNode, aiScene);

        LoadObj_VertexDeduplicated(context, data,
                                   allVertices);
    }

    void MeshLogic::LoadObj_VertexDeduplicated(Context *context,
                                               std::shared_ptr<MeshData> data,
                                               std::vector<Render::Vertex> &allVertices)
    {
        auto allVertexSize = static_cast<uint32_t>(allVertices.size());

        std::vector<Render::Vertex> vertices;
        std::vector<uint16_t> indices;
        std::unordered_map<Render::Vertex, uint16_t> uniqueVertexIndex = {};

        // TODO 水特殊处理逻辑 临时
        auto isWater = (data->info->objName == "resource/obj/water/water_30x30.obj");
        if (isWater)
        {
            for (auto i = 0u; i < allVertexSize; i++)
            {
                indices.push_back(vertices.size());
                vertices.push_back(allVertices[i]);
            }
            std::vector<Render::Vertex> originalVertices = vertices;
            for (auto i = 0u; i < allVertexSize; i += 6)
            {
                auto &v0 = originalVertices[i + 0]; // 0
                auto &v1 = originalVertices[i + 1]; // 1
                auto &v2 = originalVertices[i + 2]; // 2
                auto &v5 = originalVertices[i + 5]; // 3

                vertices[i + 0].positionOS = v0.positionOS;
                vertices[i + 0].normalOS = v1.positionOS;
                vertices[i + 0].tangentOS = v2.positionOS;
                vertices[i + 1].positionOS = v1.positionOS;
                vertices[i + 1].normalOS = v2.positionOS;
                vertices[i + 1].tangentOS = v0.positionOS;
                vertices[i + 2].positionOS = v2.positionOS;
                vertices[i + 2].normalOS = v0.positionOS;
                vertices[i + 2].tangentOS = v1.positionOS;

                vertices[i + 3].positionOS = v0.positionOS;
                vertices[i + 3].normalOS = v2.positionOS;
                vertices[i + 3].tangentOS = v5.positionOS;
                vertices[i + 4].positionOS = v2.positionOS;
                vertices[i + 4].normalOS = v5.positionOS;
                vertices[i + 4].tangentOS = v0.positionOS;
                vertices[i + 5].positionOS = v5.positionOS;
                vertices[i + 5].normalOS = v0.positionOS;
                vertices[i + 5].tangentOS = v2.positionOS;
            }
        }
        else
        {
            for (auto i = 0u; i < allVertexSize; i++)
            {
                auto &vertex = allVertices[i];
                if (uniqueVertexIndex.count(vertex) == 0)
                {
                    uniqueVertexIndex[vertex] = static_cast<uint16_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertexIndex[vertex]);
            }
        }

        data->vertices = std::move(vertices);
        data->indices = std::move(indices);
    }

    void MeshLogic::CalcBoundingSphere(Context *context,
                                       std::shared_ptr<MeshData> data)
    {
        auto minPos = glm::vec3(99999.0f);
        auto maxPos = glm::vec3(-99999.0f);

        auto &vertices = data->vertices;
        for (const auto &vertex : vertices)
        {
            auto &position = vertex.positionOS;

            minPos = glm::min(minPos, position);
            maxPos = glm::max(maxPos, position);

            // if (minPos.x > position.x)
            //     minPos.x = position.x;
            // if (minPos.y > position.y)
            //     minPos.y = position.y;
            // if (minPos.z > position.z)
            //     minPos.z = position.z;

            // if (maxPos.x < position.x)
            //     maxPos.x = position.x;
            // if (maxPos.y < position.y)
            //     maxPos.y = position.y;
            // if (maxPos.z < position.z)
            //     maxPos.z = position.z;
        }

        BoundingSphere boundingSphere = {};
        boundingSphere.center = (minPos + maxPos) * 0.5f;
        boundingSphere.radius = glm::distance(minPos, maxPos) * 0.5f * 0.8f;

        data->boundingSphere = boundingSphere;
    }

    void MeshLogic::CreateBuffer(Context *context,
                                 std::shared_ptr<MeshData> data)
    {
        auto &vertices = data->vertices;
        auto &indices = data->indices;

        auto vertexSize = static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size());
        auto indexSize = static_cast<VkDeviceSize>(sizeof(uint16_t) * indices.size());

        auto &vertexBuffer = data->vertexBuffer;
        auto &indexBuffer = data->indexBuffer;

        vertexBuffer = BufferLogic::Create(context,
                                           vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        indexBuffer = BufferLogic::Create(context,
                                          indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        BufferSetLogic::SetVector(context,
                                  vertexBuffer, vertices);
        BufferSetLogic::SetVector(context,
                                  indexBuffer, indices);
    }
}