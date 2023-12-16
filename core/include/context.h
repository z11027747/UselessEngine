
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <map>
#include "base.h"
#include "render/system.h"

class Context final {
public:
	GLFWwindow* window;

	Context(GLFWwindow* w) {
		window = w;
	}

	//render
	EngineObject renderEO;

	void Create() {
		//render
		RenderSystem::CreateGlobal(this);
		RenderSystem::CreateInstance(this);
		//RenderSystem::CreateDebugCallback(this);
		RenderSystem::CreateSurface(this);
		RenderSystem::PickupPhysicalDevice(this);
		//RenderSystem::CheckPhysicalDeviceMemory(this);
		RenderSystem::CreateLogicDevice(this);
		RenderSystem::GetLogicDeviceQueue(this);
		RenderSystem::CreateSwapchian(this);
	}

	void Update() {

	}

	void Destroy() {
		RenderSystem::DestroySwapchian(this);
		RenderSystem::DestroyLogicDevice(this);
		RenderSystem::DestroySurface(this);
		//RenderSystem::DestroyDebugCallback(this);
		RenderSystem::DestroyInstance(this);
	}

};
