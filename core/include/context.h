#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/pass/pass_comp.h"
#include "render/system.h"
#include "logic/input/input_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/system.h"
#include "editor/test.h"
#include "engine_object.h"

const std::string G_MainCamera = "MainCamera";
const std::string G_ShadowCamera = "ShadowCamera";

class Context final
{
public:
	GLFWwindow *window;
	float aspect;

	Context(GLFWwindow *w, float asp)
	{
		window = w;
		aspect = asp;
		currTime = 0.0f;
		deltaTime = 0.0f;
	}

	// time
	float currTime;
	float deltaTime;

	// logic ====================
	// render-global
	std::shared_ptr<EngineObject> renderGlobalEO;
	// render-pass
	std::shared_ptr<Render::Pass> renderImGuiPass;
	std::shared_ptr<Render::Pass> renderMainPass;
	std::shared_ptr<Render::Pass> renderShadowPass;
	// render-pipelines
	std::unordered_map<std::string, std::shared_ptr<Render::GraphicsPipeline>> renderPipelines;
	// render-cmd
	std::vector<VkCommandBuffer> renderBatchCmdBuffers;
	std::vector<VkCommandBuffer> renderTempCmdBuffers;
	// render-buffer
	std::vector<std::shared_ptr<Render::Buffer>> renderTempBuffers;
	// render-light
	std::vector<std::shared_ptr<EngineObject>> renderLightEOs;
	// render-unit
	std::vector<std::shared_ptr<EngineObject>> renderUnitEOs;

	// logic ====================
	// logic-camera
	std::shared_ptr<EngineObject> mainCameraEO;

	// all-engineObject
	std::vector<std::shared_ptr<EngineObject>> allEOs;
	std::unordered_map<std::string, std::shared_ptr<EngineObject>> allEOMap;

	void AddEO(std::shared_ptr<EngineObject> eo)
	{
		allEOs.push_back(eo);
		allEOMap.insert({eo->name, eo});

		if (eo->HasComponent<Render::Unit>())
		{
			renderUnitEOs.push_back(eo);
		}
	}

	std::shared_ptr<EngineObject> GetEO(const std::string &name)
	{
		return allEOMap[name];
	}

	void Create()
	{
		Render::System::Create(this);
		Logic::System::Create(this);

		// Test
		Editor::Test::CreateMainCamera(this);
		Editor::Test::CreateLight(this);
		Editor::Test::CreateSkybox(this);
		// Editor::Test::CreateCube(this);
		Editor::Test::CreateModel(this);
	}

	void Update()
	{
		Render::System::Update(this);
		Logic::System::Update(this);
	}

	void Destroy()
	{
		Render::System::Destroy(this);
		Logic::System::Destroy(this);
	}

	void OnSizeCallback()
	{
	}

	void OnKeyCallback(int key, int action, int mode)
	{
		std::cout << "key : " << key
				  << ", action: " << action
				  << ", mode: " << mode << std::endl;
	}
};
