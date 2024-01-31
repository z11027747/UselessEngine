
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/descriptor_comp.h"
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

        auto &postProcessPipeline = global->pipelineMap[Define::Pipeline::PostProcess_Bloom];
        auto descriptorSet = DescriptorSetLogic::AllocateOne(context, postProcessPipeline->descriptorSetLayout);

        auto &resolveImage2d = global->passMap[Define::Pass::Main]->resolveImage2d;
        VkDescriptorImageInfo imageInfo = {
            global->globalSamplerClamp,
            resolveImage2d->vkImageView,
            resolveImage2d->layout};

        auto descriptor = std::make_shared<Descriptor>();
        descriptor->set = descriptorSet;
        descriptor->imageInfos.push_back(imageInfo);

        DescriptorSetLogic::Update(context,
                                   [&descriptor](std::vector<VkWriteDescriptorSet> &writes)
                                   {
                                       DescriptorSetLogic::WriteImage(writes,
                                                                      descriptor->set, 0, descriptor->imageInfos[0]);
                                   });

        auto postProcess = std::make_shared<PostProcess>();
        postProcess->descriptor = descriptor;

        mainCameraEO->AddComponent(postProcess);
    }

    void PostProcessLogic::Destroy(Context *context, std::shared_ptr<EngineObject> mainCameraEO)
    {
    }
}