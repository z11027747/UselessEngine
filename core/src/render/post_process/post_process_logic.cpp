
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/post_process/post_process_comp.h"
#include "render/post_process/post_process_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void PostProcessLogic::Create(Context *context, std::shared_ptr<EngineObject> mainCameraEO)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto postProcess = std::make_shared<PostProcess>();

        auto &mainPass = global->passMap[Define::Pass::Main];
        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];

        auto descriptor = std::make_shared<Descriptor>();

        auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);
        descriptor->set = descriptorSet;

        VkDescriptorImageInfo resolveImageInfo = {
            global->globalSamplerClamp,
            mainPass->resolveImage2d->vkImageView,
            mainPass->resolveImage2d->layout};
        descriptor->imageInfos.push_back(resolveImageInfo);

        VkDescriptorImageInfo toonMappingAttachmentInfo = {
            global->globalSamplerClamp,
            postProcessPass->inputImage2ds[0]->vkImageView,
            postProcessPass->inputImage2ds[0]->layout};
        descriptor->imageInfos.push_back(toonMappingAttachmentInfo);

        VkDescriptorImageInfo bloomAttachmentInfo = {
            global->globalSamplerClamp,
            postProcessPass->inputImage2ds[1]->vkImageView,
            postProcessPass->inputImage2ds[1]->layout};
        descriptor->imageInfos.push_back(bloomAttachmentInfo);

        DescriptorSetLogic::Update(context,
                                   [=](std::vector<VkWriteDescriptorSet> &writes)
                                   {
                                       auto &bindings = postProcessPipeline->descriptorBindings;
                                       DescriptorSetLogic::WriteImage(writes, descriptor->set, 0,
                                                                      bindings[0].descriptorType, descriptor->imageInfos[0]);
                                       DescriptorSetLogic::WriteImage(writes, descriptor->set, 1,
                                                                      bindings[1].descriptorType, descriptor->imageInfos[1]);
                                       DescriptorSetLogic::WriteImage(writes, descriptor->set, 2,
                                                                      bindings[2].descriptorType, descriptor->imageInfos[2]);
                                   });

        postProcess->descriptor = descriptor;

        mainCameraEO->AddComponent(postProcess);
    }
    void PostProcessLogic::Destroy(Context *context, std::shared_ptr<EngineObject> mainCameraEO)
    {
    }

    void PostProcessLogic::Blit(Context *context, std::shared_ptr<EngineObject> mainCameraEO)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Global>();

        auto postProcess = mainCameraEO->GetComponent<PostProcess>();
        auto &descriptor = postProcess->descriptor;

        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        auto &inputImage2d = postProcessPass->inputImage2ds[0];

        auto &mainPass = global->passMap[Define::Pass::Main];
        auto &resolveImage2d = mainPass->resolveImage2d;

        ImageLogic::TransitionLayout(context, resolveImage2d,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 1,
                                     true);
        ImageLogic::TransitionLayout(context, inputImage2d,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, inputImage2d->mipLevels,
                                     true);

        ImageLogic::CopyFromImage(context, inputImage2d,
                                  resolveImage2d,
                                  true);

        ImageLogic::TransitionLayout(context, resolveImage2d,
                                     VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1,
                                     true);

        ImageLogic::GenerateMipmapsAndTransitionLayout(context, inputImage2d,
                                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                       true);
    }
}