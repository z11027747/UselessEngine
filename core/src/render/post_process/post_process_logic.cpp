
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
        auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];

        auto descriptor = std::make_shared<Descriptor>();

        auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);
        descriptor->set = descriptorSet;

        VkDescriptorImageInfo resolveImageInfo = {
            global->globalSamplerClamp,
            postProcessPass->colorImage2ds[1]->vkImageView,
            postProcessPass->colorImage2ds[1]->layout};
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
}