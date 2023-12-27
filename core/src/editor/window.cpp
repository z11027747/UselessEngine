
#include <imgui/imgui.h>
#include <memory>
#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_vulkan.h"
#include "editor/window.h"
#include "editor/test.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "context.h"
#include "base.h"

namespace Editor {

	void Window::Create(Context* context) {
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

		CreateStyle(context);
		Test::Create(context);
	}

	ImGuiWindowFlags windowFlags;
	int windowWidth;
	int windowHeight;
	int windowYOffset;
	int surfaceWidth;

	void Window::CreateStyle(Context* context) {
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowWidth = 300;
		windowHeight = 400;
		windowYOffset = 20;

		auto& renderGlobalEO = context->renderGlobalEO;
		auto global = renderGlobalEO->GetComponent<Render::Global>();
		surfaceWidth = global->surfaceCapabilities.currentExtent.width;
	}

	bool showDemoWindow = false;

	void Window::Update(Context* context) {

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Checkbox("DemoWindow", &showDemoWindow);
		if (showDemoWindow) {
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		DrawMenuBar(context);
		DrawHierarchy(context);
		DrawInspector(context);
	}

	void Window::DrawMenuBar(Context* context) {
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

	void Window::DrawHierarchy(Context* context) {

		ImGui::SetNextWindowPos(ImVec2(0, windowYOffset));
		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
		if (ImGui::Begin("Hierarchy", NULL, windowFlags)) {

			auto& io = ImGui::GetIO();
			ImGui::Text("fps: %.1f ms: %.3f", io.Framerate, 1000.0f / io.Framerate);

			ImGui::SeparatorText("Entity List");

			ImGui::Text("[id=0] Camera");
			ImGui::SameLine();
			if (ImGui::Button("Select")) {}
			ImGui::SameLine();
			if (ImGui::Button("Delete")) {}

			ImGui::Text("[id=1] Cube");
			ImGui::SameLine();
			if (ImGui::Button("Select")) {}
			ImGui::SameLine();
			if (ImGui::Button("Delete")) {}

			ImGui::SeparatorText("End");
			if (ImGui::Button("Add Entity")) {}

			ImGui::End();
		}
	}

	float position[3] = { 0.0f, 0.0f, 0.0f };
	float eulerAngles[3] = { -30.0f, 50.0f, -30.0f };
	float scale[3] = { 1.0f, 1.0f, 1.0f };

	static char shaderName[128] = "test";
	static char textureName[128] = "icon2.png";

	void Window::DrawInspector(Context* context) {
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoCollapse;
		windowFlags |= ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos(ImVec2(surfaceWidth - windowWidth, windowYOffset));
		ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
		if (ImGui::Begin("Inspector", NULL, windowFlags)) {

			ImGui::Text("Select: Cube");

			ImGui::SeparatorText("Transform");
			ImGui::InputFloat3("Position", position);
			ImGui::InputFloat3("EulerAngles", eulerAngles);
			ImGui::InputFloat3("Scale", scale);
			ImGui::Spacing();

			ImGui::SeparatorText("Render");
			ImGui::InputText("ShaderName", shaderName, IM_ARRAYSIZE(shaderName));
			ImGui::InputText("TextureName", textureName, IM_ARRAYSIZE(textureName));
			ImGui::Spacing();

			ImGui::SeparatorText("End");
			if (ImGui::Button("Add Component")) {}

			ImGui::End();
		}
	}

	void Window::DrawData(Context* context, uint32_t imageIndex) {
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

	void Window::Destroy(Context* context) {

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}