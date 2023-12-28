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
#include "render/system.h"
#include "logic/system.h"
#include "engine_object.h"

const std::string G_Camera = "Camera";

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
	//render-pipelines
	std::unordered_map<std::string, std::shared_ptr<Render::GraphicsPipeline>> renderPipelines;
	//render-cmdBuffer/buffer
	std::shared_ptr<Render::CmdSimple> renderCmdSimple;
	std::vector<std::shared_ptr<Render::Buffer>> renderTempBuffers;
	//render-unit
	std::vector<std::shared_ptr<EngineObject>> renderUnitEOs;

	//all-engineObject
	std::vector<std::shared_ptr<EngineObject>> allEOs;
	std::unordered_map<std::string, std::shared_ptr<EngineObject>> allEOMap;

	void AddEO(std::shared_ptr<EngineObject> eo) {
		allEOs.emplace_back(eo);
		allEOMap.emplace(eo->name, eo);
	}

	std::shared_ptr<EngineObject> GetEO(const std::string& name) {
		return allEOMap[name];
	}

	void Create() {

		Logic::System::Create(this);
		Render::System::Create(this);
	}

	void Update() {

		Logic::System::Update(this);
		Render::System::Update(this);
	}

	void Destroy() {

		Logic::System::Destroy(this);
		Render::System::Destroy(this);
	}

	void OnSizeCallback() {

	}

};
