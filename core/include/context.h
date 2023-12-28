#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/cmd/cmd_comp.h"
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

	//render-global
	std::shared_ptr<EngineObject> renderGlobalEO;
	//render-pipeline
	std::unordered_map<std::string, std::shared_ptr<Render::GraphicsPipeline>> renderPipelines;
	//render-buffer
	std::shared_ptr<Render::CmdSimple> renderCmdSimple;
	std::vector<std::shared_ptr<Render::Buffer>> renderTempBuffers;
	//render-unit
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
