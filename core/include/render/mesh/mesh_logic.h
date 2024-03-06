#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <json/json11.hpp>
#include "render/mesh/mesh_comp.h"

class Context;
class EngineObject;

namespace Render
{
    class MeshLogic final
    {
    public:
        static void OnAdd(Context *, std::shared_ptr<EngineObject>);
        static void OnRemove(Context *, std::shared_ptr<EngineObject>);

        static bool IsShared(Context *, std::shared_ptr<Mesh>);

        static void TryDestroyEO(Context *, std::shared_ptr<EngineObject>);
        static void SetDestroy(Context *, std::shared_ptr<MeshData>);
        static void Destroy(Context *, std::shared_ptr<MeshData>);

        static std::shared_ptr<MeshData> Get(Context *, std::shared_ptr<MeshInfo>);
        static std::shared_ptr<MeshData> Create(Context *, std::shared_ptr<MeshInfo>);

        static void LoadObj_TinyObj(Context *, std::shared_ptr<MeshData>);
        static void LoadObj_Assimp(Context *, std::shared_ptr<MeshData>);
        static void LoadObj_VertexDeduplicated(Context *, std::shared_ptr<MeshData>,
                                               std::vector<Render::Vertex> &);

        static void CalcBoundingSphere(Context *, std::shared_ptr<MeshData>);
        static void CreateBuffer(Context *, std::shared_ptr<MeshData>);

        static void CreateCache(Context *);
        static void DestroyCache(Context *);

        static void CreateVertexInstances(Context *);
        static void UpdateVertexInstanceBuffer(Context *);
        static void DestroyVertexInstances(Context *);
    };

    class MeshJson final
    {
    public:
        static std::shared_ptr<void> Parse(const json11::Json &);
        static json11::Json Serialize(std::shared_ptr<void>);
    };
}