
#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <memory>
#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_vulkan.h"
#include "editor/global.h"
#include "editor/window/hierachy_window.h"
#include "editor/window/inspector_window.h"
#include "editor/wrap/engine_object_wrap.h"
#include "editor/test.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "context.h"
#include "engine_object.h"

namespace Editor {

	void Global::Create(Context* context) {
		auto& window = context->window;

		auto& renderGlobalEO = context->renderGlobalEO;
		auto global = renderGlobalEO->GetComponent<Render::Global>();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsClassic();
		auto& style = ImGui::GetStyle();
		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.SeparatorTextAlign = { 1.0f, 0.5f };

		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = global->instance;
		init_info.PhysicalDevice = global->physicalDevice;
		init_info.Device = global->logicalDevice;
		init_info.QueueFamily = global->physicalQueueFamilyIndex;
		init_info.Queue = global->logicalQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = global->descriptorPool;
		init_info.Subpass = 0;
		init_info.MinImageCount = global->surfaceCapabilities.minImageCount;
		init_info.ImageCount = global->maxFrameInFlight;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = VK_NULL_HANDLE;
		init_info.CheckVkResultFn = VK_NULL_HANDLE;

		ImGui_ImplVulkan_Init(&init_info, global->renderPass);

		Test::Create(context);
	}

	void Global::Update(Context* context) {
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool showDemoWindow = false;
		ImGui::Checkbox("DemoWindow", &showDemoWindow);
		if (showDemoWindow) {
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		DrawMenuBar(context);

		HierachyWindow::Draw(context);
		InspectorWindow::Draw(context);
	}

	void Global::DrawMenuBar(Context* context) {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save As..")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void Global::RenderData(Context* context, uint32_t imageIndex) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Render::Global>();
		auto& cmdBuffer = global->cmdBuffers[imageIndex];

		ImGui::Render();
		auto* main_draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(main_draw_data, cmdBuffer);

		auto& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void Global::Destroy(Context* context) {
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}