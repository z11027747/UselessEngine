
#include <vulkan/vulkan.h>
#include <memory>
#include "render/vk/global/global_comp.h"
#include "render/vk/logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/render_pass/render_pass_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_logic.h"
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

        ImGui_ImplVulkan_Init(&init_info, global->passes[Render::Pass_ImGui]->renderPass);
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

        VkDescriptorSetLayoutBinding samplerBinding0 = {};
        samplerBinding0.binding = 0;
        samplerBinding0.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerBinding0.descriptorCount = 1;
        samplerBinding0.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.push_back(samplerBinding0);

        auto descriptorSetLayout = Render::DescriptorSetLayoutLogic::Create(context, bindings);

        auto &colorImage2d = global->passes[Render::Pass_Main]->colorImage2ds[0];
        // auto &colorImage2d = global->passes[Render::Pass_Shadow]->depthImage2ds[0];

        auto descriptorSet = Render::DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);

        auto descriptor = std::make_shared<Render::Descriptor>();
        descriptor->set = descriptorSet;

        VkDescriptorImageInfo imageInfo = {
            global->globalSamplerClamp,
            colorImage2d->vkImageView,
            colorImage2d->layout};

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

    void TestLogic::CreateMainCamera(Context *context)
    {
        auto mainCameraEO = std::make_shared<EngineObject>();
        mainCameraEO->name = Name_MainCamera;

        Logic::TransformLogic::Add(mainCameraEO,
                                   glm::vec3(0.7f, 1.49743f, -3.979505f),
                                   glm::vec3(-1.0f, 0.0f, 0.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f));

        auto mainCamera = std::make_shared<Logic::Camera>();
        mainCamera->near = 0.1f;
        mainCamera->far = 100.0f;
        mainCamera->mode = Logic::CameraMode::ePerspective;
        mainCamera->fov = 60.0f;
        mainCamera->passName = Render::Pass_Main;
        mainCameraEO->AddComponent<Logic::Camera>(mainCamera);

        context->AddEO(mainCameraEO);
    }

    void TestLogic::CreateLight(Context *context)
    {
        auto directionLightEO = std::make_shared<EngineObject>();
        directionLightEO->name = Name_DirectionLight;

        Logic::TransformLogic::Add(directionLightEO,
                                   glm::vec3(-5.0f, 5.0f, 0.0f),
                                   glm::vec3(45.0f, 45.0f, 0.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f));

        auto directionLight = std::make_shared<Render::DirectionLight>();
        directionLight->color = glm::vec3(1.0f, 0.9568627f, 0.8392157f);
        directionLight->hasShadow = true;

        directionLightEO->AddComponent(directionLight);

        auto directionLightCamera = std::make_shared<Logic::Camera>();
        directionLightCamera->near = 0.1f;
        directionLightCamera->far = 50.0f;
        directionLightCamera->mode = Logic::CameraMode::eOrtho;
        directionLightCamera->size = 30.0f;
        directionLightCamera->passName = Render::Pass_Shadow;
        directionLightEO->AddComponent<Logic::Camera>(directionLightCamera);

        context->AddEO(directionLightEO);
    }

    void TestLogic::CreateCubes(Context *context)
    {
        return;

        CreateCube(context, glm::vec3(0.0f, 0.0f, 0.0f), -90.0f,
                   "resource/texture/arena/ground01_arena_ds.tga", // albedo
                   "resource/texture/arena/ground01_arena_ds.tga", //
                   "resource/texture/arena/ground01_arena_n.tga"   // normalMap
        );
    }

    void TestLogic::CreateCube(Context *context,
                               const glm::vec3 &pos, float eulerAngleY,
                               const std::string &imageAlbedoName, const std::string &imageSpecularName, const std::string &imageNormalMapName,
                               bool castShadow)
    {
        auto modelEO = std::make_shared<EngineObject>();
        modelEO->name = "ground01_arena";

        Logic::TransformLogic::Add(modelEO,
                                   pos,
                                   glm::vec3(0.0f, eulerAngleY, 0.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f));

        auto modelMesh = std::make_shared<Render::Mesh>();
        modelMesh->objName = "resource/obj/arena/ground01_arena.obj";
        modelMesh->checkHit = true;

        auto modelMaterial = std::make_shared<Render::Material>();
        modelMaterial->info = std::make_shared<Render::MaterialInfo>();
        modelMaterial->info->pipelineName = Render::Pipeline_LightModel;
        modelMaterial->info->imageNames = {imageAlbedoName, imageSpecularName, imageNormalMapName};
        modelMaterial->info->params = {glm::vec4(1.0f, 100.0f, 1.0f, 0.0f)};
        modelMaterial->info->castShadow = castShadow;

        modelEO->AddComponent(modelMesh);
        modelEO->AddComponent(modelMaterial);

        context->AddEO(modelEO);
    }

    void TestLogic::CreateAxis(Context *context)
    {
        auto modelParentEO = std::make_shared<EngineObject>();
        modelParentEO->name = Name_Axis;
        modelParentEO->active = false;
        modelParentEO->hideInHierarchy = true;

        Logic::TransformLogic::Add(modelParentEO,
                                   glm::vec3(1.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(0.5f, 0.5f, 0.5f));

        context->AddEO(modelParentEO);

        {
            auto modelEO = std::make_shared<EngineObject>();
            modelEO->name = Name_AxisX;
            modelEO->hideInHierarchy = true;

            Logic::TransformLogic::Add(modelEO,
                                       glm::vec3(1.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 90.0f, 0.0f),
                                       glm::vec3(1.0f, 1.0f, 1.0f));
            Logic::TransformLogic::SetParent(modelEO, modelParentEO);

            auto modelMesh = std::make_shared<Render::Mesh>();
            modelMesh->objName = "resource/obj/base/axis.obj";
            modelMesh->vertexColor = glm::vec3(1.0f, 0.0f, 0.0f);
            modelMesh->checkHit = true;

            auto modelMaterial = std::make_shared<Render::Material>();
            modelMaterial->info = std::make_shared<Render::MaterialInfo>();
            modelMaterial->info->pipelineName = Render::Pipeline_Color;

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);

            context->AddEO(modelEO);
        }

        {
            auto modelEO = std::make_shared<EngineObject>();
            modelEO->name = Name_AxisY;
            modelEO->hideInHierarchy = true;

            Logic::TransformLogic::Add(modelEO,
                                       glm::vec3(0.0f, 1.0f, 0.0f),
                                       glm::vec3(-90.0f, 0.0f, 0.0f),
                                       glm::vec3(1.0f, 1.0f, 1.0f));
            Logic::TransformLogic::SetParent(modelEO, modelParentEO);

            auto modelMesh = std::make_shared<Render::Mesh>();
            modelMesh->objName = "resource/obj/base/axis.obj";
            modelMesh->vertexColor = glm::vec3(0.0f, 1.0f, 0.0f);
            modelMesh->checkHit = true;

            auto modelMaterial = std::make_shared<Render::Material>();
            modelMaterial->info = std::make_shared<Render::MaterialInfo>();
            modelMaterial->info->pipelineName = Render::Pipeline_Color;

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);

            context->AddEO(modelEO);
        }

        {
            auto modelEO = std::make_shared<EngineObject>();
            modelEO->name = Name_AxisZ;
            modelEO->hideInHierarchy = true;

            Logic::TransformLogic::Add(modelEO,
                                       glm::vec3(0.0f, 0.0f, 1.0f),
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(1.0f, 1.0f, 1.0f));
            Logic::TransformLogic::SetParent(modelEO, modelParentEO);

            auto modelMesh = std::make_shared<Render::Mesh>();
            modelMesh->objName = "resource/obj/base/axis.obj";
            modelMesh->vertexColor = glm::vec3(0.0f, 0.0f, 1.0f);
            modelMesh->checkHit = true;

            auto modelMaterial = std::make_shared<Render::Material>();
            modelMaterial->info = std::make_shared<Render::MaterialInfo>();
            modelMaterial->info->pipelineName = Render::Pipeline_Color;

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);

            context->AddEO(modelEO);
        }
    }
}
