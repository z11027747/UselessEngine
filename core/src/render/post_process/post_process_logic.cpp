
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

        auto &postProcessPass = global->passMap[Define::Pass::PostProcess];
        auto &colorImage2d = postProcessPass->colorImage2ds[1];

        auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];
        auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);

        VkDescriptorImageInfo imageInfo = {
            global->globalSamplerClamp,
            colorImage2d->vkImageView,
            colorImage2d->layout};

        auto descriptor = std::make_shared<Descriptor>();
        descriptor->set = descriptorSet;
        descriptor->imageInfos.push_back(imageInfo);

        DescriptorSetLogic::Update(context,
                                   [&descriptor](std::vector<VkWriteDescriptorSet> &writes)
                                   {
                                       DescriptorSetLogic::WriteImage(writes,
                                                                      descriptor->set, 0, descriptor->imageInfos[0]);
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
        auto &colorImage2d = postProcessPass->colorImage2ds[1];

        auto &mainPass = global->passMap[Define::Pass::Main];
        auto &resolveImage2d = mainPass->resolveImage2d;

        ImageLogic::TransitionLayout(context, resolveImage2d,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 1,
                                     true);
        ImageLogic::TransitionLayout(context, colorImage2d,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, colorImage2d->mipLevels,
                                     true);

        ImageLogic::CopyFromImage(context, colorImage2d,
                                  resolveImage2d,
                                  true);

        ImageLogic::TransitionLayout(context, resolveImage2d,
                                     VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1,
                                     true);

        ImageLogic::GenerateMipmapsAndTransitionLayout(context, colorImage2d,
                                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                       true);
    }
}