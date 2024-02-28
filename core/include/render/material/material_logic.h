#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/material_comp.h"

class Context;
class EngineObject;

namespace Render
{
    class MaterialLogic final
    {
    public:
        static void OnAdd(Context *, std::shared_ptr<EngineObject>);
        static void OnRemove(Context *, std::shared_ptr<EngineObject>);

        static void TryDestroyEO(Context *, std::shared_ptr<EngineObject>);
        static void SetDestroy(Context *, std::shared_ptr<MaterialData>);
        static void Destroy(Context *, std::shared_ptr<MaterialData>);

        static std::shared_ptr<MaterialData> Create(Context *,
                                                    const std::string &);
        static std::shared_ptr<MaterialData> Create(Context *,
                                                    std::shared_ptr<MaterialInfo>);

        static void GetOrCreateImage(Context *,
                                     std::shared_ptr<MaterialData>, const std::string &);
        static void GetOrCreateImageCube(Context *,
                                         std::shared_ptr<MaterialData>, const std::vector<std::string> &);

        static void CreateImage(Context *,
                                std::shared_ptr<MaterialData>, const std::string &);
        static void CreateImageCube(Context *,
                                    std::shared_ptr<MaterialData>, const std::vector<std::string> &);

        static void CreateBuffer(Context *, std::shared_ptr<MaterialData>, size_t);
        static void CreateBuffer(Context *, std::shared_ptr<MaterialData>);
        static void CreateDescriptor(Context *, std::shared_ptr<MaterialData>);

        static void CreateCache(Context *);
        static void DestroyCache(Context *);
    };

    class MaterialPipelineLogic final
    {
    public:
        static void SetVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetViewport(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetMultisampleCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetDepthStencilCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
        static void SetColorBlendStage(Context *, std::shared_ptr<GraphicsPipeline>);
    };

    class MaterialDescriptorLogic final
    {
    public:
        static void CreateSetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void DestroySetLayout(Context *, std::shared_ptr<GraphicsPipeline>);
        static void AllocateAndUpdate(Context *, std::shared_ptr<MaterialData>);
        static void Update(Context *, std::shared_ptr<MaterialData>);
        static void Destroy(Context *, std::shared_ptr<MaterialData>);
    };

    class MaterialGlobalDescriptorLogic final
    {
    public:
        static void Create(Context *);
        static void Destroy(Context *);
    };
}