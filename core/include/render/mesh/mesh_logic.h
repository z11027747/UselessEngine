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
        static std::shared_ptr<Mesh> CreateByObj(Context *,
                                                 const std::string &, float, glm::vec3 & = glm::vec3(1.0f, 1.0f, 1.0f));

        static void Destroy(Context *, std::shared_ptr<Mesh>);

        static void SetObj(Context *, std::shared_ptr<Mesh>,
                           const std::string &, float, glm::vec3 & = glm::vec3(1.0f, 1.0f, 1.0f));

        static void CreateBuffer(Context *, std::shared_ptr<Mesh>);

        static void CalcBoundingSphere(Context *, std::shared_ptr<Mesh>);
    };
}