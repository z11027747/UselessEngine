
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/buffer/buffer_set_logic.h"
#include "common/res_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    constexpr int instanceCount = 1024;
    void MeshLogic::CreateVertexInstances(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto meshCache = globalEO->GetComponent<MeshCache>();

        auto &vertexInstances = meshCache->vertexInstances;
        vertexInstances.resize(instanceCount);

        auto vertexInstanceSize = static_cast<VkDeviceSize>(sizeof(VertexInstance) * vertexInstances.size());

        auto &vertexInstanceBuffer = meshCache->vertexInstanceBuffer;
        vertexInstanceBuffer = BufferLogic::Create(context,
                                                   vertexInstanceSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
    void MeshLogic::UpdateVertexInstanceBuffer(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto meshCache = globalEO->GetComponent<MeshCache>();

        auto &vertexInstances = meshCache->vertexInstances;
        auto &vertexInstanceBuffer = meshCache->vertexInstanceBuffer;

        // TODO 不用整个都更新
        BufferSetLogic::SetVector(context,
                                  vertexInstanceBuffer, vertexInstances);
    }

}