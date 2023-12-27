#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "render/system_new.h"
#include "base.h"

class Context final {
public:
	GLFWwindow* window;
	float aspect;

	Context(GLFWwindow* w, float asp) {
		window = w;
		aspect = asp;
		currTime = 0.0f;
		deltaTime = 0.0f;
	}

	//time
	float currTime;
	float deltaTime;

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

	}

};
