#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include "render/mesh/mesh_comp.h"

class Context;

namespace Render
{
    class MeshLogic final
    {
    public:
        static std::shared_ptr<Mesh> Create(Context *, const std::string &, glm::vec3 &defaultColor = glm::vec3(1.0f, 1.0f, 1.0f));
        static void Destroy(Context *);

        static void SetVerticesBuffer(Context *, std::shared_ptr<Mesh>);
        static void SetIndicesBuffer(Context *, std::shared_ptr<Mesh>);
    };

}