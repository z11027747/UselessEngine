#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "vulkan/vulkan.h"
#include "render/vk/global/global_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/material/material_logic.h"
#include "render/render_pass/render_pass_system.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "common/define.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    static std::unordered_map<std::string, std::vector<std::shared_ptr<EngineObject>>> materialEOMap{};

    static void SplitPipeline(Context *);
    static void SortPipeline();
    static void DrawPipeline(Context *,
                             uint32_t, bool,
                             const std::string &);

    void RenderPassSystem::Update(Context *context,
                                  uint32_t imageIndex, bool isShadow)
    {
        auto &cameraEO = context->logicMainCameraEO;
        if (cameraEO == nullptr)
            return;

        SplitPipeline(context);
        SortPipeline();

        auto camera = cameraEO->GetComponent<Logic::Camera>();
        auto &cameraPass = camera->passName;

        if (cameraPass == Define::Pass::Forward)
        {
            DrawPipeline(context, imageIndex, isShadow, Define::Pipeline::LightModel);
            DrawPipeline(context, imageIndex, isShadow, Define::Pipeline::Color);
        }
        else if (cameraPass == Define::Pass::Deferred)
        {
            DrawPipeline(context, imageIndex, isShadow, Define::Pipeline::Deferred_LightModel);
        }
    }

    static void SplitPipeline(Context *context)
    {
        materialEOMap.clear();

        auto &materialEOs = context->renderMaterialEOs;
        for (const auto &materialEO : materialEOs)
        {
            if (!materialEO->active)
                continue;

            auto transform = materialEO->GetComponent<Logic::Transform>();
            auto &parentEOName = transform->parentEOName;
            if (!parentEOName.empty() && !context->GetEO(parentEOName)->active)
                continue;

            auto material = materialEO->GetComponent<Render::Material>();
            auto &materialInfo = material->info;
            if (materialInfo->pipelineName.empty())
                continue;

            materialEOMap[materialInfo->pipelineName].push_back(materialEO);
        }
    }

    static void SortPipeline()
    {
        for (auto &kv : materialEOMap)
        {
            auto &eos = kv.second;
            std::sort(eos.begin(), eos.end(),
                      [](const std::shared_ptr<EngineObject> &eol, const std::shared_ptr<EngineObject> &eor)
                      {
                          auto ml = eol->GetComponent<Render::Material>();
                          auto mr = eor->GetComponent<Render::Material>();
                          return ml->info->renderQueue < mr->info->renderQueue;
                      });
        }
    }

    static void DrawPipeline(Context *context,
                             uint32_t imageIndex, bool isShadow,
                             const std::string &pipelineName)
    {
        auto it = materialEOMap.find(pipelineName);
        if (it == materialEOMap.end())
            return;

        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();
        auto &vkCmdBuffer = global->swapchainCmdBuffers[imageIndex];

        auto &graphicsPipeline = global->pipelineMap[!isShadow ? pipelineName : Define::Pipeline::Shadow];
        auto &pipeline = graphicsPipeline->pipeline;
        auto &pipelineLayout = graphicsPipeline->pipelineLayout;
        vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        auto &materialEOs = materialEOMap[pipelineName];
        for (const auto &materialEO : materialEOs)
        {
            auto transform = materialEO->GetComponent<Logic::Transform>();
            auto &model = transform->model;

            auto mesh = materialEO->GetComponent<Render::Mesh>();
            auto &meshInstance = mesh->instance;
            auto &meshInfo = mesh->info;

            auto material = materialEO->GetComponent<Render::Material>();
            auto &materialInstance = material->instance;
            auto &materialInfo = material->info;
            if (isShadow && !materialInfo->castShadow)
                continue;

            auto vertexBuffer = meshInstance->vertexBuffer->vkBuffer;
            auto indexBuffer = meshInstance->indexBuffer->vkBuffer;
            auto indexSize = static_cast<uint32_t>(meshInstance->indices.size());

            VkBuffer vertexBuffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(vkCmdBuffer, 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(vkCmdBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdPushConstants(vkCmdBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &model);

            std::vector<VkDescriptorSet> descriptorSets;
            descriptorSets.push_back(global->globalDescriptor->set);

            if (!isShadow)
            {
                if (materialInstance->descriptor != nullptr)
                    descriptorSets.push_back(materialInstance->descriptor->set);
            }

            vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                    static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(),
                                    0, nullptr);

            vkCmdDrawIndexed(vkCmdBuffer, indexSize, 1, 0, 0, 0);
        }
    }

};