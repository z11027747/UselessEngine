
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

		//TODO
		Test::Create(context);

		auto& window = context->window;

		auto& renderGlobalEO = context->renderGlobalEO;
		auto global = renderGlobalEO->GetComponent<Render::Global>();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui::StyleColorsDark();

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
	}

	void Window::Update(Context* context) {

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static float f = 0.0f;
		static int counter = 0;
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		if (ImGui::Button("Button")) {
			counter++;
		}
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		ImGui::Text("average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();

		ImGui::Render();
	}

	void Window::DrawData(Context* context, uint32_t imageIndex) {
		auto& renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Render::Global>();
		auto& cmdBuffer = global->cmdBuffers[imageIndex];

		ImDrawData* draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(draw_data, cmdBuffer);
	}

}