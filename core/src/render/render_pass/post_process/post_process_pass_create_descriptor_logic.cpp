
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/pass/pass_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    constexpr int imageCount = 3; // blit + toonmapping+bloom

    void RenderPassLogic::CreatePostProcessDescriptor(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Global];

        auto descriptor = std::make_shared<Descriptor>();

        auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);
        descriptor->set = descriptorSet;

        VkDescriptorImageInfo blitImageInfo = {
            global->globalSamplerClamp,
            postProcessPass->colorImage2ds[1]->vkImageView,
            postProcessPass->colorImage2ds[1]->layout};
        descriptor->imageInfos.push_back(blitImageInfo);

        for (auto i = 0; i < imageCount - 1; i++)
        {
            VkDescriptorImageInfo inputAttachmentInfo = {
                global->globalSamplerClamp,
                postProcessPass->inputImage2ds[i]->vkImageView,
                postProcessPass->inputImage2ds[i]->layout};
            descriptor->imageInfos.push_back(inputAttachmentInfo);
        }

        DescriptorSetLogic::Update(context,
                                   [=](std::vector<VkWriteDescriptorSet> &writes)
                                   {
                                       auto &bindings = postProcessPipeline->descriptorBindings;
                                       for (auto i = 0; i < imageCount; i++)
                                       {
                                           DescriptorSetLogic::WriteImage(writes, descriptor->set, i,
                                                                          bindings[i].descriptorType, descriptor->imageInfos[i]);
                                       }
                                   });

        descriptorMap[Define::Pass::PostProcess] = descriptor;
    }
}
