#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/material_comp.h"

class Context;

namespace Render
{
    class MaterialLogic final
    {
    public:
        static void Create(Context *, std::shared_ptr<Material>);
        static void Destroy(Context *, std::shared_ptr<Material>);

        static void CreateImage(Context *, std::shared_ptr<Material>);
        static void CreateImageCube(Context *, std::shared_ptr<Material>);
        static void CreateDescriptor(Context *, std::shared_ptr<Material>);
    };

    class MaterialPipelineLogic final
    {
    public:
        static void SetVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetViewport(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetColorBlendStage(Context *, std::shared_ptr<GraphicsPipeline>);
    };

    class MaterialDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void DestroySetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void AllocateAndUpdate(Context *, std::shared_ptr<Material>);
        static void Destroy(Context *, std::shared_ptr<Material>);
    };
}