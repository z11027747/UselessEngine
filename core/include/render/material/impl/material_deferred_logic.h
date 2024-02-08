#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "render/material/material_comp.h"

class Context;

namespace Render
{
    class MaterialDeferredVolumnPipelineLogic final
    {
    public:
        static void SetVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetColorBlendStage(Context *, std::shared_ptr<GraphicsPipeline>);
    };

    class MaterialDeferredVolumnDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
    };

    class MaterialDeferredPointLightPipelineLogic final
    {
    public:
        static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetMultisampleCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
    };

    class MaterialDeferredPointLightDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
    };

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
        static void AllocateAndUpdate(Context *, std::shared_ptr<MaterialInstance>);
        static void Destroy(Context *, std::shared_ptr<MaterialInstance>);
    };
}