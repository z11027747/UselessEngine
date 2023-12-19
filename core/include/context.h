
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include "base.h"
#include "render/system.h"
#include "tool/system.h"

class Context final {
public:
	GLFWwindow* window;

	Context(GLFWwindow* w) {
		window = w;
	}

	//render
	std::shared_ptr<EngineObject> renderEO;

	void Create() {
		//render
		RenderSystem::CreateGlobal(this);
		RenderSystem::CreateInstance(this);
		RenderSystem::CreateDebugCallback(this);
		RenderSystem::CreateSurface(this);
		RenderSystem::PickupPhysicalDevice(this);
		RenderSystem::CreateLogicDevice(this);
		RenderSystem::GetLogicDeviceQueue(this);
		RenderSystem::CreateSwapchian(this);
		RenderSystem::GetSwapchianImages(this);
		RenderSystem::CreateSwapchianImageViews(this);
		RenderSystem::CreateShader(this, "test");
		RenderSystem::CreatePipelineLayout(this);
		RenderSystem::CreateRenderPass(this);
		RenderSystem::CreateGraphicsPipeline(this);
		RenderSystem::CreateFrameBuffers(this);
		RenderSystem::CreateCommandPool(this);
		RenderSystem::AllocateSwapchainCommandBuffers(this);
		RenderSystem::CreateSemaphores(this);
		RenderSystem::CreateFences(this);
		//RenderSystem::CheckPhysicalDeviceMemory(this);
		//RenderSystem::CreateVertexTriangle(this);
		RenderSystem::CreateVertexRectangle(this);
		//RenderSystem::CreateVertexBufferHost(this);
		RenderSystem::CreateVertexBufferStageing(this);
		RenderSystem::CreateIndexBufferHost(this);
	}

	void Update() {
		RenderSystem::DrawFrame(this);
		RenderSystem::TryRecreateSwapchain(this);
	}

	void Destroy() {
		RenderSystem::WaitIdle(this);

		RenderSystem::DestroyIndexBuffer(this);
		RenderSystem::DestroyVertexBuffer(this);
		RenderSystem::DestroyFences(this);
		RenderSystem::DestroySemaphores(this);
		RenderSystem::DestroyCommandPool(this);
		RenderSystem::DestroyFrameBuffers(this);
		RenderSystem::DestroyGraphicsPipeline(this);
		RenderSystem::DestroyRenderPass(this);
		RenderSystem::DestroyPipelineLayout(this);
		RenderSystem::DestroyAllShaders(this);
		RenderSystem::DestroySwapchianImageViews(this);
		RenderSystem::DestroySwapchian(this);
		RenderSystem::DestroyLogicDevice(this);
		RenderSystem::DestroySurface(this);
		RenderSystem::DestroyDebugCallback(this);
		RenderSystem::DestroyInstance(this);
	}

	void OnSizeCallback() {
		RenderSystem::SetNeedRecreateSwapchain(this);
	}

};
