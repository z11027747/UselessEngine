
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
#include "render/mesh/mesh_logic.h"
#include "render/material/material_logic.h"
#include "render/unit/unit_comp.h"
#include "render/light/light_comp.h"
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_logic.h"
#include "editor/test_logic.h"
#include "editor/system.h"
#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_vulkan.h"
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
        descriptor->image0Info = {
            global->globalSampler,
            colorImage2d->vkImageView,
            colorImage2d->layout};

        Render::DescriptorSetLogic::Update(context,
                                           [&](std::vector<VkWriteDescriptorSet> &writes)
                                           {
                                               Render::DescriptorSetLogic::WriteImage(context,
                                                                                      writes,
                                                                                      descriptor);
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
                                   glm::vec3(7.68f, 5.77f, -3.12f),
                                   glm::vec3(26.41f, -55.35f, 0.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f));

        auto mainCamera = std::make_shared<Logic::Camera>();
        mainCamera->near = 0.1f;
        mainCamera->far = 100.0f;
        mainCamera->mode = Logic::CameraMode::ePerspective;
        mainCamera->fov = 50.0f;
        mainCamera->size = 15.0f;
        mainCamera->passName = Render::Pass_Main;
        mainCameraEO->AddComponent<Logic::Camera>(mainCamera);

        context->logicMainCameraEO = mainCameraEO;
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
        directionLight->params = glm::vec4(1.0f, 15.0f, 1.0f, 0.0f);
        directionLight->hasShadow = true;

        directionLightEO->AddComponent(directionLight);

        auto directionLightCamera = std::make_shared<Logic::Camera>();
        directionLightCamera->near = 0.1f;
        directionLightCamera->far = 50.0f;
        directionLightCamera->mode = Logic::CameraMode::eOrtho;
        directionLightCamera->size = 30.0f;
        directionLightCamera->passName = Render::Pass_Shadow;
        directionLightEO->AddComponent<Logic::Camera>(directionLightCamera);

        auto directionLightUnit = std::make_shared<Render::Unit>();

        context->AddEO(directionLightEO);
        context->renderLightEOs.emplace_back(directionLightEO);
    }

    void TestLogic::CreateSkybox(Context *context)
    {
        auto skyboxEO = std::make_shared<EngineObject>();
        skyboxEO->name = Name_Skybox;

        Logic::TransformLogic::Add(skyboxEO,
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, -100.0f, 0.0f),
                                   glm::vec3(50.0f, 50.0f, 50.0f));

        auto skyboxMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/base/sphere.obj", 1.0f);
        auto skyboxMaterial = Render::MaterialLogic::CreateByImageCube(context, Render::Pipeline_Skybox,
                                                                       {"resource/texture/skybox2/sky_left_lnitial.png",
                                                                        "resource/texture/skybox2/sky_right_lnitial.png",
                                                                        "resource/texture/skybox2/sky_up_lnitial.png",
                                                                        "resource/texture/skybox2/sky_down_lnitial.png",
                                                                        "resource/texture/skybox2/sky_front_lnitial.png",
                                                                        "resource/texture/skybox2/sky_back_lnitial.png"});
        auto skyboxUnit = std::make_shared<Render::Unit>();

        skyboxEO->AddComponent(skyboxMesh);
        skyboxEO->AddComponent(skyboxMaterial);
        skyboxEO->AddComponent(skyboxUnit);

        context->AddEO(skyboxEO);
        context->renderUnitEOs.emplace_back(skyboxEO);
    }

    void TestLogic::CreateCubes(Context *context)
    {
        CreateCube(context, glm::vec3(-4.0f, 0.0f, 0.0f), -90.0f, "resource/texture/cube_world/SoilWGrass_3_D.jpg");
        CreateCube(context, glm::vec3(-3.0f, 0.0f, 0.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(-2.0f, 0.0f, 0.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(-1.0f, 0.0f, 0.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(2.0f, 0.0f, 0.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(3.0f, 0.0f, 0.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(4.0f, 0.0f, 0.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_3_D.jpg");

        CreateCube(context, glm::vec3(-4.0f, 0.0f, 1.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(-3.0f, 0.0f, 1.0f), 0.0f, "resource/texture/cube_world/Ground_D.png");
        CreateCube(context, glm::vec3(-2.0f, -0.5f, 1.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(-1.0f, -0.5f, 1.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(0.0f, -0.5f, 1.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(1.0f, -0.5f, 1.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(2.0f, -0.5f, 1.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(3.0f, -0.5f, 1.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(4.0f, 0.0f, 1.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");

        CreateCube(context, glm::vec3(-3.0f, 0.0f, 2.0f), 0.0f, "resource/texture/cube_world/Ground_D.png");
        CreateCube(context, glm::vec3(-2.0f, -0.5f, 2.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(-1.0f, -0.5f, 2.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(0.0f, -0.5f, 2.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(1.0f, -0.5f, 2.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(2.0f, -0.5f, 2.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(3.0f, -0.5f, 2.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(4.0f, 0.0f, 2.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");

        CreateCube(context, glm::vec3(-3.0f, 0.0f, 3.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-3.0f, 1.0f, 3.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-2.0f, 0.0f, 3.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-2.0f, 1.0f, 3.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-1.0f, -0.5f, 3.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(0.0f, -0.5f, 3.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(1.0f, 0.0f, 3.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(2.0f, 0.0f, 3.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(3.0f, 0.0f, 3.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(4.0f, 0.0f, 3.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");

        CreateCube(context, glm::vec3(-3.0f, 0.0f, 4.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-3.0f, 1.0f, 4.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-2.0f, 0.0f, 4.0f), 0.0f, "resource/texture/cube_world/SoilWGrass_6_D.jpg");
        CreateCube(context, glm::vec3(-1.0f, 0.0f, 4.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(0.0f, -0.5f, 4.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(1.0f, 0.0f, 4.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(2.0f, 0.0f, 4.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(3.0f, 0.0f, 4.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(4.0f, 0.0f, 4.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");

        CreateCube(context, glm::vec3(-3.0f, 0.0f, 5.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-2.0f, 0.0f, 5.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-1.0f, -0.5f, 5.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(0.0f, -0.5f, 5.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(1.0f, 0.0f, 5.0f), 0.0f, "resource/texture/cube_world/Ground_D.png");
        CreateCube(context, glm::vec3(2.0f, 0.0f, 5.0f), 0.0f, "resource/texture/cube_world/Ground_D.png");
        CreateCube(context, glm::vec3(3.0f, 0.0f, 5.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(4.0f, 0.0f, 5.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");

        CreateCube(context, glm::vec3(-4.0f, 0.0f, 6.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(-3.0f, 0.0f, 6.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(-2.0f, -0.5f, 6.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(-1.0f, -0.5f, 6.0f), 0.0f, "resource/texture/cube_world/Water.png");
        CreateCube(context, glm::vec3(0.0f, 0.0f, 6.0f), 0.0f, "resource/texture/cube_world/GroundWGrass_D.jpg");
        CreateCube(context, glm::vec3(1.0f, 0.0f, 6.0f), 0.0f, "resource/texture/cube_world/Ground_D.png");
        CreateCube(context, glm::vec3(2.0f, 0.0f, 6.0f), 0.0f, "resource/texture/cube_world/Ground_D.png");
        CreateCube(context, glm::vec3(3.0f, 0.0f, 6.0f), 0.0f, "resource/texture/cube_world/GroundWGrass2_D.jpg");
        CreateCube(context, glm::vec3(4.0f, 0.0f, 6.0f), 0.0f, "resource/texture/cube_world/Ground_D.png");

        CreateCube(context, glm::vec3(-4.0f, 0.0f, 7.0f), 180.0f, "resource/texture/cube_world/SoilWGrass_3_D.jpg");
        CreateCube(context, glm::vec3(-3.0f, 0.0f, 7.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(-2.0f, 0.0f, 7.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(-1.0f, 0.0f, 7.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(0.0f, 0.0f, 7.0f), 90.0f, "resource/texture/cube_world/SoilWGrass_5_D.jpg");
        CreateCube(context, glm::vec3(1.0f, 0.0f, 7.0f), 0.0f, "resource/texture/cube_world/GroundWGrass2_D.jpg");
        CreateCube(context, glm::vec3(2.0f, 0.0f, 7.0f), 0.0f, "resource/texture/cube_world/GroundWGrass2_D.jpg");
        CreateCube(context, glm::vec3(3.0f, 0.0f, 7.0f), 0.0f, "resource/texture/cube_world/GroundWGrass2_D.jpg");
        CreateCube(context, glm::vec3(4.0f, 0.0f, 7.0f), 0.0f, "resource/texture/cube_world/Ground_D.png");
    }

    void TestLogic::CreateCube(Context *context,
                               const glm::vec3 &pos, float eulerAngleY, const std::string &imageName)
    {
        auto modelEO = std::make_shared<EngineObject>();
        modelEO->name = "Cube";

        Logic::TransformLogic::Add(modelEO,
                                   pos,
                                   glm::vec3(0.0f, eulerAngleY, 0.0f),
                                   glm::vec3(1.0f, 1.0f, 1.0f));

        auto modelMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/cube_world/cube.obj", 0.01f);
        auto modelMaterial = Render::MaterialLogic::CreateByImage(context, Render::Pipeline_Bling_Phone, imageName);
        modelMaterial->castShadow = true;
        auto modelUnit = std::make_shared<Render::Unit>();

        modelEO->AddComponent(modelMesh);
        modelEO->AddComponent(modelMaterial);
        modelEO->AddComponent(modelUnit);

        context->AddEO(modelEO);
        context->renderUnitEOs.emplace_back(modelEO);
        context->logicHitEOs.emplace_back(modelEO);
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

            auto modelMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/base/axis.obj", 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
            modelMesh->bound.center = glm::vec3(-0.2f, 0.0f, 0.0f);
            modelMesh->bound.radius = 0.3f;
            auto modelMaterial = Render::MaterialLogic::CreateByImage(context, Render::Pipeline_Color, "resource/obj/viking_room/viking_room.png");
            auto modelUnit = std::make_shared<Render::Unit>();

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);
            modelEO->AddComponent(modelUnit);

            context->AddEO(modelEO);
            context->renderUnitEOs.emplace_back(modelEO);
            context->logicAxisHitEOs.emplace_back(modelEO);
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

            auto modelMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/base/axis.obj", 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            modelMesh->bound.center = glm::vec3(0.0f, -0.2f, 0.0f);
            modelMesh->bound.radius = 0.3f;
            auto modelMaterial = Render::MaterialLogic::CreateByImage(context, Render::Pipeline_Color, "resource/obj/viking_room/viking_room.png");
            auto modelUnit = std::make_shared<Render::Unit>();

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);
            modelEO->AddComponent(modelUnit);

            context->AddEO(modelEO);
            context->renderUnitEOs.emplace_back(modelEO);
            context->logicAxisHitEOs.emplace_back(modelEO);
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

            auto modelMesh = Render::MeshLogic::CreateByObj(context, "resource/obj/base/axis.obj", 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            modelMesh->bound.center = glm::vec3(0.0f, 0.0f, -0.2f);
            modelMesh->bound.radius = 0.3f;
            auto modelMaterial = Render::MaterialLogic::CreateByImage(context, Render::Pipeline_Color, "resource/obj/viking_room/viking_room.png");
            auto modelUnit = std::make_shared<Render::Unit>();

            modelEO->AddComponent(modelMesh);
            modelEO->AddComponent(modelMaterial);
            modelEO->AddComponent(modelUnit);

            context->AddEO(modelEO);
            context->renderUnitEOs.emplace_back(modelEO);
            context->logicAxisHitEOs.emplace_back(modelEO);
        }
    }
}
