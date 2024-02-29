#pragma once

#include <algorithm>
#include "vulkan/vulkan.h"
#include "render/vk/global/global_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/material/material_logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "define.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    static std::unordered_map<std::string, std::vector<std::shared_ptr<EngineObject>>> materialEOMap{};

    static void SplitPipeline(Context *);
    static void SortPipeline();
    static void DrawPipeline(Context *, bool,
                             const std::string &);

    void RenderPassLogic::Draw(Context *context, bool isShadow)
    {
        auto &mainCameraEO = context->logicMainCameraEO;
        if (mainCameraEO == nullptr)
            return;

        SplitPipeline(context);
        SortPipeline();

        auto camera = mainCameraEO->GetComponent<Logic::Camera>();
        auto &cameraPass = camera->passName;

        if (cameraPass == Define::Pass::Forward)
        {
            DrawPipeline(context, isShadow, Define::Pipeline::Skybox);
            DrawPipeline(context, isShadow, Define::Pipeline::LightModel);
            DrawPipeline(context, isShadow, Define::Pipeline::PBR_Simplest);
            DrawPipeline(context, isShadow, Define::Pipeline::Dissolve);
            DrawPipeline(context, isShadow, Define::Pipeline::Color);
            DrawPipeline(context, isShadow, Define::Pipeline::Water);
        }
        else if (cameraPass == Define::Pass::Deferred)
        {
            DrawPipeline(context, isShadow, Define::Pipeline::Deferred_Volumn);
            DrawPipeline(context, isShadow, Define::Pipeline::Deferred_LightModel);
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

    static VkDeviceSize defaultOffset[] = {0};

    static void DrawPipeline(Context *context, bool isShadow,
                             const std::string &pipelineName)
    {
        auto it = materialEOMap.find(pipelineName);
        if (it == materialEOMap.end())
            return;

        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto currFrame = global->currFrame;
        auto &vkCmdBuffer = global->swapchainCmdBuffers[currFrame];

        auto &graphicsPipeline = global->pipelineMap[!isShadow ? pipelineName : Define::Pipeline::Shadow];
        auto &pipeline = graphicsPipeline->pipeline;
        auto &pipelineLayout = graphicsPipeline->pipelineLayout;
        vkCmdBindPipeline(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        auto &materialEOs = materialEOMap[pipelineName];
        for (const auto &materialEO : materialEOs)
        {
            auto transform = materialEO->GetComponent<Logic::Transform>();
            auto &transformModel = transform->model;

            auto mesh = materialEO->GetComponent<Render::Mesh>();
            auto &meshInfo = mesh->info;
            auto &meshData = mesh->data;

            auto material = materialEO->GetComponent<Render::Material>();
            auto &materialInfo = material->info;
            auto &materialData = material->data;
            if (isShadow && !materialInfo->castShadow)
                continue;

            if (material->useInstance)
                continue;

            auto vertexBuffer = meshData->vertexBuffer;
            vkCmdBindVertexBuffers(vkCmdBuffer, 0, 1, &vertexBuffer->vkBuffer, defaultOffset);

            auto indexBuffer = meshData->indexBuffer;
            vkCmdBindIndexBuffer(vkCmdBuffer, indexBuffer->vkBuffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdPushConstants(vkCmdBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transformModel);

            std::vector<VkDescriptorSet> descriptorSets;
            descriptorSets.push_back(global->globalDescriptor->set);

            if (!isShadow)
            {
                if (materialData->descriptor != nullptr)
                    descriptorSets.push_back(materialData->descriptor->set);
            }

            vkCmdBindDescriptorSets(vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0,
                                    static_cast<uint32_t>(descriptorSets.size()), descriptorSets.data(),
                                    0, nullptr);

            auto indexSize = static_cast<uint32_t>(meshData->indices.size());
            vkCmdDrawIndexed(vkCmdBuffer, indexSize, 1, 0, 0, 0);
        }
    }

};