
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/render_pass/render_pass_comp.h"
#include "editor/test_logic.h"
#include "editor/system.h"
#include "editor/imgui_demo/imgui_impl_glfw.h"
#include "editor/imgui_demo/imgui_impl_vulkan.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
    void TestLogic::CreateImGui(Context *context)
    {
        auto &window = context->window;

        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Render::Global>();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        auto &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsClassic();
        auto &style = ImGui::GetStyle();
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.SeparatorTextAlign = {1.0f, 0.5f};

        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = global->instance;
        init_info.PhysicalDevice = global->physicalDevice;
        init_info.Device = global->logicalDevice;
        init_info.QueueFamily = global->physicalQueueFamilyIndex;
        init_info.Queue = global->logicalQueue;
        init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = global->descriptorPool;
        init_info.Subpass = 0;
        init_info.MinImageCount = global->surfaceCapabilities.minImageCount;
        init_info.ImageCount = global->swapchainImageCount;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = Render::CheckRet;

        ImGui_ImplVulkan_Init(&init_info, global->passMap[Render::Pass_ImGui]->renderPass);
    }

    void TestLogic::DestroyImGui(Context *context)
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void TestLogic::CreateDescriptor(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Render::Global>();
        auto &logicalDevice = global->logicalDevice;
        auto &currentExtent = global->surfaceCapabilities.currentExtent;
        auto swapchainImageCount = global->swapchainImageCount;

        VkDescriptorSetLayoutBinding samplerBinding0 = {
            0,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            1,
            VK_SHADER_STAGE_FRAGMENT_BIT};

        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.push_back(samplerBinding0);

        auto descriptorSetLayout = Render::DescriptorSetLayoutLogic::Create(context, bindings);

        // auto &image2d = global->passMap[Render::Pass_Main]->colorImage2ds[0];
        auto &image2d = global->passMap[Render::Pass_Main]->resolveImage2d;
        // auto &image2d = global->passMap[Render::Pass_Shadow]->depthImage2d;

        auto descriptorSet = Render::DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);

        auto descriptor = std::make_shared<Render::Descriptor>();
        descriptor->set = descriptorSet;

        VkDescriptorImageInfo imageInfo = {
            global->globalSamplerClamp,
            image2d->vkImageView,
            image2d->layout};

        descriptor->imageInfos.push_back(imageInfo);

        Render::DescriptorSetLogic::Update(context,
                                           [&descriptor](std::vector<VkWriteDescriptorSet> &writes)
                                           {
                                               Render::DescriptorSetLogic::WriteImage(writes,
                                                                                      descriptor->set, 0, descriptor->imageInfos[0]);
                                           });

        System::descriptorSetLayout = descriptorSetLayout;
        System::descriptor = descriptor;
    }

    void TestLogic::DestroyDescriptor(Context *context)
    {
        auto &descriptorSetLayout = System::descriptorSetLayout;
        auto &descriptor = System::descriptor;

        Render::DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
    }
}
