
#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <memory>
#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_vulkan.h"
#include "editor/system.h"
#include "editor/window.h"
#include "editor/test.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/cmd/cmd_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{
	void System::Create(Context *context)
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
		init_info.CheckVkResultFn = nullptr;

		ImGui_ImplVulkan_Init(&init_info, global->passes[Render::Pass_ImGui]->renderPass);

		CreateDescriptors(context);
	}

	VkDescriptorSetLayout System::descriptorSetLayout = nullptr;
	std::vector<std::shared_ptr<Render::Descriptor>> System::descriptors = {};

	void System::CreateDescriptors(Context *context)
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

		descriptorSetLayout = Render::DescriptorSetLayoutLogic::Create(context, bindings);

		for (auto i = 0u; i < swapchainImageCount; i++)
		{
			auto &colorImage2d = global->passes[Render::Pass_Main]->colorImage2ds[i];

			auto descriptorSet = Render::DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);
			auto sampler = Render::SamplerLogic::Create(context);

			VkDescriptorImageInfo vkImageInfo = {
				sampler,
				colorImage2d->vkImageView,
				colorImage2d->layout};

			auto descriptor = std::make_shared<Render::Descriptor>();
			descriptor->set = descriptorSet;
			descriptor->vkImage0Info = vkImageInfo;

			Render::DescriptorSetLogic::Update(context,
											   [&](std::vector<VkWriteDescriptorSet> &writes)
											   {
												   Render::DescriptorSetLogic::WriteImage(context,
																						  writes,
																						  descriptor);
											   });

			descriptors.push_back(descriptor);
		}
	}

	void System::DestroyDescriptors(Context *context)
	{
		Render::DescriptorSetLayoutLogic::Destroy(context, descriptorSetLayout);
		for (auto &descriptor : descriptors)
		{
			Render::SamplerLogic::Destroy(context, descriptor->vkImage0Info.sampler);
		}
	}

	void System::NewFrame(Context *context)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void System::Draw(Context *context, uint32_t imageIndex)
	{
		static bool showDemoWindow = true;
		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
			// return;
		}

		const auto *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		bool p_open = true;
		if (ImGui::Begin("##DockSpace-FullScreen", &p_open, window_flags))
		{
			auto &io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				auto dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
			}

			Window::Draw(context, imageIndex);

			ImGui::End();
		}

		ImGui::PopStyleVar(2);
	}

	void System::Render(Context *context, VkCommandBuffer &vkCmdBuffer)
	{
		ImGui::Render();
		auto *main_draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(main_draw_data, vkCmdBuffer);

		auto &io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void System::Destroy(Context *context)
	{
		DestroyDescriptors(context);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}