
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    constexpr int imageCount = 6; // shadow + gBuffer(position+normal+color+material+pointlight)

    void RenderPassLogic::CreateDeferredDescriptor(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &deferredPass = global->passMap[Define::Pass::Deferred];
        auto &deferredLightingPipeline = global->pipelineMap[Define::Pipeline::Deferred_Shading];

        auto descriptor = std::make_shared<Descriptor>();

        auto descriptorSet = DescriptorSetLogic::AllocateOne(context, deferredLightingPipeline->descriptorSetLayout);
        descriptor->set = descriptorSet;

        auto &shadowPass = global->passMap[Define::Pass::Shadow];
        auto depthImageSampler = SamplerLogic::CreateDepth(context);

        VkDescriptorImageInfo shadowImageInfo = {
            depthImageSampler,
            shadowPass->depthImage2d->vkImageView,
            shadowPass->depthImage2d->layout};
        descriptor->imageInfos.push_back(shadowImageInfo);

        for (auto i = 0; i < imageCount - 1; i++)
        {
            VkDescriptorImageInfo inputAttachmentInfo = {
                global->globalSamplerClamp,
                deferredPass->inputImage2ds[i]->vkImageView,
                deferredPass->inputImage2ds[i]->layout};
            descriptor->imageInfos.push_back(inputAttachmentInfo);
        }

        DescriptorSetLogic::Update(context,
                                   [=](std::vector<VkWriteDescriptorSet> &writes)
                                   {
                                       auto &bindings = deferredLightingPipeline->descriptorBindings;
                                       for (auto i = 0; i < imageCount; i++)
                                       {
                                           DescriptorSetLogic::WriteImage(writes, descriptor->set, i,
                                                                          bindings[i].descriptorType, descriptor->imageInfos[i]);
                                       }
                                   });

        descriptorMap[Define::Pass::Deferred] = descriptor;
    }
}
