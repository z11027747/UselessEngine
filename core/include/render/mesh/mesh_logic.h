#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "render/mesh/mesh_comp.h"

class Context;
class EngineObject;

namespace Render
{
    class MeshLogic final
    {
    public:
        static bool IsShared(Context *, std::shared_ptr<Mesh>);
        static void TryDestroyEO(Context *, std::shared_ptr<EngineObject>);
    };

    class MeshInstanceLogic final
    {
    public:
        static void CreateCache(Context *context);
        static void DestroyCache(Context *context);

        static std::shared_ptr<MeshInstance> Get(Context *, std::shared_ptr<MeshInfo>);
        static std::shared_ptr<MeshInstance> Create(Context *, std::shared_ptr<MeshInfo>);

        static void Destroy(Context *, std::shared_ptr<MeshInstance>);
        static void SetDestroy(Context *, std::shared_ptr<MeshInstance>);

        static void LoadObj(Context *, std::shared_ptr<MeshInstance>);
        static void CalcBoundingSphere(Context *, std::shared_ptr<MeshInstance>);
        static void CreateBuffer(Context *, std::shared_ptr<MeshInstance>);
    };
}