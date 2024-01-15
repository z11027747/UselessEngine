#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "render/mesh/mesh_comp.h"

class Context;

namespace Render
{
    class MeshLogic final
    {
    public:
        static void Create(Context *, std::shared_ptr<Mesh>);
        static void Destroy(Context *, std::shared_ptr<Mesh>);

        static void LoadObj(Context *, std::shared_ptr<Mesh>);
        static void CreateBuffer(Context *, std::shared_ptr<Mesh>);
        static void CalcBoundingSphere(Context *, std::shared_ptr<Mesh>);
    };
}