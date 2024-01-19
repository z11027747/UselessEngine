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
    };

    class MaterialInstanceLogic final
    {
    public:
        static void CreateCache(Context *);
        static void DestroyCache(Context *);

        static std::shared_ptr<MaterialInstance> Get(Context *,
                                                     const std::string &,
                                                     const std::vector<std::string> &, bool);
        static std::shared_ptr<MaterialInstance> Create(Context *,
                                                        const std::string &,
                                                        const std::vector<std::string> &, bool);
        static void Destroy(Context *, std::shared_ptr<MaterialInstance>);
        static void SetDestroy(Context *, std::shared_ptr<MaterialInstance>);

        static void CreateImage(Context *,
                                std::shared_ptr<MaterialInstance>,
                                const std::string &);
        static void CreateImageCube(Context *,
                                    std::shared_ptr<MaterialInstance>,
                                    const std::vector<std::string> &);

        static void CreateBuffer(Context *,
                                 std::shared_ptr<MaterialInstance>);

        static void CreateDescriptor(Context *context,
                                     std::shared_ptr<MaterialInstance>);
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

    class MaterialGlobalDescriptorLogic final
    {
    public:
        static void Create(Context *);
        static void Destroy(Context *);
    };

    class MaterialDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void DestroySetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void AllocateAndUpdate(Context *, std::shared_ptr<MaterialInstance>);
        static void Update(Context *, std::shared_ptr<MaterialInstance>);
        static void Destroy(Context *, std::shared_ptr<MaterialInstance>);
    };
}