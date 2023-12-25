#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

#include "render/system_new.h"
#include "render/system.h"
#include "tool/system.h"
#include "base.h"

class Context final {
public:
	GLFWwindow* window;

	Context(GLFWwindow* w) {
		window = w;
		currTime = 0.0f;
		deltaTime = 0.0f;
	}

	//time
	float currTime;
	float deltaTime;

	//render_old
	std::shared_ptr<EngineObject> renderEO;

	//render
	std::shared_ptr<EngineObject> renderGlobalEO;
	std::shared_ptr<EngineObject> renderCmdSimpleEO;
	std::unordered_map<std::string, std::shared_ptr<EngineObject>> renderPipelineEOs;
	std::vector<std::shared_ptr<EngineObject>> renderUnitEOs;

	//camera
	std::shared_ptr<EngineObject> cameraEO;

	void Create() {

		Render::System::OnCreate(this);

	}

	void Update() {

		Render::System::OnUpdate(this);

	}

	void Destroy() {

		Render::System::OnDestroy(this);

	}

	void OnSizeCallback() {
		RenderSystem::SetNeedRecreateSwapchain(this);
	}

};
