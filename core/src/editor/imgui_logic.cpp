
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "common/define.h"
#include "editor/imgui_logic.h"
#include "editor/system.h"
#include "editor/imgui_demo/imgui_impl_glfw.h"
#include "editor/imgui_demo/imgui_impl_vulkan.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{

    void ImGuiLogic::CreateImGui(Context *context)
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
        style.AntiAliasedLines = true;
        style.AntiAliasedLinesUseTex = false;
        style.AntiAliasedFill = false;

        auto *colors = style.Colors;
        // colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.63f, 0.0f, 1.0f);

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

        ImGui_ImplVulkan_Init(&init_info,
                              global->passMap[Define::Pass::ImGui]->renderPass);
    }

    void ImGuiLogic::DestroyImGui(Context *context)
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    VkDescriptorSetLayout ImGuiLogic::descriptorSetLayout = nullptr;

    void ImGuiLogic::CreateDescriptorSetLayout(Context *context)
    {
        VkDescriptorSetLayoutBinding samplerBinding0 = {
            0,
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            1,
            VK_SHADER_STAGE_FRAGMENT_BIT};

        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.push_back(samplerBinding0);

        descriptorSetLayout = Render::DescriptorSetLayoutLogic::Create(context, bindings);
    }

    void ImGuiLogic::DestroyDescriptorSetLayout(Context *context)
    {
        Render::DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
    }

    std::shared_ptr<Render::Descriptor> ImGuiLogic::CreateDescriptor(Context *context, std::shared_ptr<Render::Image> image)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Render::Global>();

        auto descriptorSet = Render::DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);

        auto descriptor = std::make_shared<Render::Descriptor>();
        descriptor->set = descriptorSet;

        VkDescriptorImageInfo imageInfo = {
            global->globalSamplerClamp,
            image->vkImageView,
            image->layout};
        descriptor->imageInfos.push_back(imageInfo);

        Render::DescriptorSetLogic::Update(context,
                                           [=](std::vector<VkWriteDescriptorSet> &writes)
                                           {
                                               Render::DescriptorSetLogic::WriteImage(writes,
                                                                                      descriptor->set, 0,
                                                                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptor->imageInfos[0]);
                                           });

        return descriptor;
    }

    std::unordered_map<std::string, std::shared_ptr<Render::Descriptor>> ImGuiLogic::descriptorMap = {};

    void ImGuiLogic::CreateDescriptors(Context *context)
    {
        auto &globalEO = context->renderGlobalEO;
        auto global = globalEO->GetComponent<Render::Global>();
        {
            auto &resolveImage2d = global->passMap[Define::Pass::Forward]->resolveImage2d;
            descriptorMap[Define::Pass::Forward] = CreateDescriptor(context, resolveImage2d);
        }
        {
            auto &depthImage2d = global->passMap[Define::Pass::Shadow]->depthImage2d;
            descriptorMap[Define::Pass::Shadow] = CreateDescriptor(context, depthImage2d);
        }
        {
            auto &colorImage2ds = global->passMap[Define::Pass::PostProcess]->colorImage2ds[0];
            descriptorMap[Define::Pass::PostProcess] = CreateDescriptor(context, colorImage2ds);
        }
    }

    VkDescriptorSet ImGuiLogic::GetDescriptorSet(const std::string &passName)
    {
        return descriptorMap[passName]->set;
    }
}
