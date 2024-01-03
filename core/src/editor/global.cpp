
#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <memory>
#include "editor/imgui/imgui_impl_glfw.h"
#include "editor/imgui/imgui_impl_vulkan.h"
#include "editor/global.h"
#include "editor/window.h"
#include "editor/wrap/engine_object_wrap.h"
#include "editor/test.h"
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Editor
{

	void Global::Create(Context *context)
	{
		auto &window = context->window;

		auto &renderGlobalEO = context->renderGlobalEO;
		auto global = renderGlobalEO->GetComponent<Render::Global>();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		auto &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
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

		ImGui_ImplVulkan_Init(&init_info, context->renderMainPass->renderPass);
	}

	static std::shared_ptr<Render::Image> imguiViewportImage2d = nullptr;
	static VkSampler imguiViewportImageSampler = nullptr;

	void Global::CreateViewport(Context *context)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Render::Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		Render::ImageInfo imguiViewportImage2dInfo = {
			VK_FORMAT_R8G8B8A8_UNORM, {currentExtent.width, currentExtent.height, 0}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			// memory
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};

		imguiViewportImage2d = Render::ImageLogic::CreateByInfo(context, imguiViewportImage2dInfo);
		imguiViewportImageSampler = Render::SamplerLogic::Create(context);
	}

	VkDescriptorSet GetTexture(Context *context,
							   VkSampler sampler, VkImageView image_view, VkImageLayout image_layout)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &descriptorSetLayout = context->renderPipelines["bling_phone"]->descriptorSetLayout;

		// Create Descriptor Set:
		VkDescriptorSet descriptor_set = Render::DescriptorSetLogic::AllocateOne(context, descriptorSetLayout);

		// Update the Descriptor Set:
		VkDescriptorImageInfo desc_image[1] = {};
		desc_image[0].sampler = sampler;
		desc_image[0].imageView = image_view;
		desc_image[0].imageLayout = image_layout;
		VkWriteDescriptorSet write_desc[1] = {};
		write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_desc[0].dstSet = descriptor_set;
		write_desc[0].descriptorCount = 1;
		write_desc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write_desc[0].pImageInfo = desc_image;
		vkUpdateDescriptorSets(logicalDevice, 1, write_desc, 0, nullptr);

		return descriptor_set;
	}

	void FreeTexture(Context *context, VkDescriptorSet descriptorSet)
	{
		auto &renderGlobalEO = context->renderGlobalEO;

		auto global = renderGlobalEO->GetComponent<Render::Global>();
		auto &logicalDevice = global->logicalDevice;
		auto &descriptorPool = global->descriptorPool;

		vkFreeDescriptorSets(logicalDevice, descriptorPool, 1, &descriptorSet);
	}

	void Global::NewFrame(Context *context)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool showDemoWindow = false;
		ImGui::Checkbox("DemoWindow", &showDemoWindow);
		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		const auto *viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		bool p_open = true;
		ImGui::Begin("##DockSpace", &p_open, window_flags);

		ImGui::PopStyleVar(3);

		auto &io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		Window::Draw(context);

		ImGui::End();

		ImGui::Begin("Viewport");

		auto &renderGlobalEO = context->renderGlobalEO;
		auto global = renderGlobalEO->GetComponent<Render::Global>();

		auto &currFrameImage = global->swapchainImages[global->currFrame];

		// VkCommandBuffer commandBuffer = beginSingleTimeCommands();
		// transitionImageLayout(commandBuffer, swapchainImages[imageIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
		// transitionImageLayout(commandBuffer, intermediateImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		// copyImage(commandBuffer, swapchainImages[imageIndex], intermediateImage, width, height);
		// transitionImageLayout(commandBuffer, intermediateImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		// endSingleTimeCommands(commandBuffer);

		// auto vkDescriptorSet = GetTexture(context, vkSampler, vkImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		// ImGui::Image(vkDescriptorSet, ImVec2{viewportPanelSize.x, viewportPanelSize.y});
		// FreeTexture(context, vkDescriptorSet);

		ImGui::End();
	}

	void Global::RenderDrawData(Context *context,
								uint32_t imageIndex, VkCommandBuffer &vkCmdBuffer)
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

	void Global::Destroy(Context *context)
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

}