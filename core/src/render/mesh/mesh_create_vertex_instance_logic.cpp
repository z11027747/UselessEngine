
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
    void MeshInstanceLogic::CreateVertexInstances(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MeshInstanceCache>();

        auto &vertexInstances = instanceCache->vertexInstances;
        vertexInstances.resize(instanceCount);

        auto vertexInstanceSize = static_cast<VkDeviceSize>(sizeof(VertexInstance) * vertexInstances.size());

        auto &vertexInstanceBuffer = instanceCache->vertexInstanceBuffer;
        vertexInstanceBuffer = BufferLogic::Create(context,
                                                   vertexInstanceSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
    void MeshInstanceLogic::UpdateVertexInstanceBuffer(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto instanceCache = globalEO->GetComponent<MeshInstanceCache>();

        auto &vertexInstances = instanceCache->vertexInstances;
        auto &vertexInstanceBuffer = instanceCache->vertexInstanceBuffer;

        // TODO 
        BufferSetLogic::SetVector(context,
                                  vertexInstanceBuffer,
                                  vertexInstances);
    }

}