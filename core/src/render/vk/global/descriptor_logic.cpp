
#include <vector>
#include "render/vk/global/global_comp.h"
#include "render/vk/global/global_logic.h"
#include "render/vk/logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/material/material_comp.h"
#include "context.h"
#include "engine_object.h"

namespace Render
{
    void DescriptorLogic::CreateGlobal(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Render::Global>();
        auto &logicalDevice = global->logicalDevice;
        auto swapchainImageCount = global->swapchainImageCount;

        // set layout
        VkDescriptorSetLayoutBinding globalUniformBinding0 = {};
        globalUniformBinding0.binding = 0;
        globalUniformBinding0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        globalUniformBinding0.descriptorCount = 1;
        globalUniformBinding0.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        std::vector<VkDescriptorSetLayoutBinding> globalBindings;
        globalBindings.push_back(globalUniformBinding0);

        auto globalDescriptorSetLayout = DescriptorSetLayoutLogic::Create(context, globalBindings);

        // descriptor
        auto globalDescriptor = std::make_shared<Descriptor>();

        auto globalDescriptorSet = DescriptorSetLogic::AllocateOne(context, globalDescriptorSetLayout);
        globalDescriptor->set = globalDescriptorSet;

        auto globalUBOSize = sizeof(GlobalUBO);
        auto globalBuffer = BufferLogic::Create(context,
                                                globalUBOSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        globalDescriptor->bufferInfo = {
            globalBuffer->vkBuffer,
            0,
            globalUBOSize};

        global->globalDescriptorSetLayout = globalDescriptorSetLayout;
        global->globalDescriptor = globalDescriptor;
        global->globalBuffer = globalBuffer;

        // update
        DescriptorSetLogic::Update(context,
                                   [&globalDescriptor](std::vector<VkWriteDescriptorSet> &writes)
                                   {
                                       DescriptorSetLogic::WriteBuffer(writes,
                                                                       globalDescriptor->set, 0, globalDescriptor->bufferInfo);
                                   });

        global->globalSampler = SamplerLogic::Create(context);
    }

    void DescriptorLogic::DestroyGlobal(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Render::Global>();

        DescriptorSetLayoutLogic::Destroy(context, global->globalDescriptorSetLayout);
        BufferLogic::Destroy(context, global->globalBuffer);

        SamplerLogic::Destroy(context, global->globalSampler);
    }

}