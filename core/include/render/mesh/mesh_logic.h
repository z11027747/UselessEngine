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
    };

    class MeshInstanceLogic final
    {
    public:
        static void CreateCache(Context *context);
        static void DestroyCache(Context *context);

        static std::shared_ptr<MeshInstance> Get(Context *, const std::string &);
        static std::shared_ptr<MeshInstance> Create(Context *,
                                                    const std::string &, const glm::vec3 & = glm::vec3(1.0f));
        static void Destroy(Context *, std::shared_ptr<MeshInstance>);
        static void SetDestroy(Context *, std::shared_ptr<MeshInstance>);

        static void LoadObj(Context *,
                            std::shared_ptr<MeshInstance>,
                            const std::string &, const glm::vec3 &);
        static void CalcBoundingSphere(Context *, std::shared_ptr<MeshInstance>);
        static void CreateBuffer(Context *, std::shared_ptr<MeshInstance>);
    };
}