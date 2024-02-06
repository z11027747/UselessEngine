#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "render/material/material_comp.h"

class Context;

namespace Render
{
    class MaterialDeferredShadingPipelineLogic final
    {
    public:
        static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetMultisampleCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
    };

    class MaterialDeferredShadingDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
    };
}