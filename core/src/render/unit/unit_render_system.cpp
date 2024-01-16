#pragma once

#include "vulkan/vulkan.h"
#include "render/vk/global/global_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/material/material_logic.h"
#include "render/unit/unit_comp.h"
#include "render/unit/unit_system.h"
#include "logic/transform/transform_comp.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void UnitRenderSystem::Update(Context *context,
                                  uint32_t imageIndex, bool isShadow)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

        auto &unitEOs = context->renderUnitEOs;
        for (const auto &unitEO : unitEOs)
        {
            if (!unitEO->active)
                continue;

            auto unitTransform = unitEO->GetComponent<Logic::Transform>();

            auto &unitParentEOName = unitTransform->parentEOName;
            if (!unitParentEOName.empty())
            {
                auto unitParentEO = context->GetEO(unitParentEOName);
                if (!unitParentEO->active)
                    continue;
            }

            auto material = unitEO->GetComponent<Render::Material>();
            if (isShadow && !material->castShadow)
                continue;

            auto pipelineName = !isShadow ? material->pipelineName : "shadow";

            auto &graphicsPipeline = global->pipelines[pipelineName];
            auto &pipeline = graphicsPipeline->pipeline;
            auto &pipelineLayout = graphicsPipeline->pipelineLayout;

            vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

            auto mesh = unitEO->GetComponent<Render::Mesh>();
            auto &meshInstance = mesh->instance;

            VkBuffer vertexBuffer = meshInstance->vertexBuffer->vkBuffer;
            VkBuffer indexBuffer = meshInstance->indexBuffer->vkBuffer;
            uint32_t indexSize = static_cast<uint32_t>(meshInstance->indices.size());

            VkBuffer vertexBuffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(vkCmdBuffer, 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(vkCmdBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            auto &model = unitTransform->model;
            vkCmdPushConstants(vkCmdBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &model);

            std::vector<VkDescriptorSet> descriptorSets;
            descriptorSets.push_back(global->globalDescriptor->set);

            if (!isShadow)
            {
                if (material->descriptor != nullptr)
                    descriptorSets.push_back(material->descriptor->set);
            }

            vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pipelineLayout, 0, static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(), 0, nullptr);

            vkCmdDrawIndexed(vkCmdBuffer, indexSize, 1, 0, 0, 0);
        }
    }
};