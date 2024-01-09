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
    class MaterialBlingPhonePipelineLogic final
    {
    public:
        static void SetVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetViewport(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetColorBlendStage(Context *, std::shared_ptr<GraphicsPipeline>);
    };

    class MaterialBlingPhoneDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void DestroySetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void AllocateAndUpdate(Context *, std::shared_ptr<Material>);
        static void Destroy(Context *, std::shared_ptr<Material>);
    };
}