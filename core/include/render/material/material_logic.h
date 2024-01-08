#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <memory>
#include <string>
#include "render/material/material_comp.h"

class Context;

namespace Render
{
    class MaterialLogic final
    {
    public:
        static std::shared_ptr<Material> Create(Context *);
        static void Destroy(Context *);

        static void SetPipelineName(Context *, std::shared_ptr<Material>, const std::string &);
        static void SetImage(Context *, std::shared_ptr<Material>, const std::string &);
        static void SetImageCube(Context *, std::shared_ptr<Material>, std::array<std::string, 6>);

        static void SetImageBuffer(Context *, std::shared_ptr<Material>);
    };

}