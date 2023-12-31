#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "render/material/material_comp.h"

class Context;

namespace Render
{
    class MaterialSkyboxPipelineLogic final
    {
    public:
        static void SetVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetViewport(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetColorBlendStage(Context *, std::shared_ptr<GraphicsPipeline>);
    };

    class MaterialSkyboxDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void DestroySetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void AllocateAndUpdate(Context *, std::shared_ptr<Material>);
        static void Destroy(Context *, std::shared_ptr<Material>);
    };
}