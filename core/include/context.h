#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "render/system.h"
#include "logic/system.h"
#include "editor/system.h"
#include "editor/test_logic.h"
#include "engine_object.h"

const std::string Name_MainCamera = "MainCamera";
const std::string Name_DirectionLight = "DirectionLight";
const std::string Name_Skybox = "Skybox";
const std::string Name_Axis = "Axis";

class Context final
{
public:
	GLFWwindow *window;
	int width;
	int height;
	float aspect;

	Context(GLFWwindow *win, int w, int h, float asp)
	{
		window = win;
		width = w;
		height = h;
		aspect = asp;
		currTime = 0.0f;
		deltaTime = 0.0f;
	}

	// time
	float currTime;
	float deltaTime;

	// render
	std::shared_ptr<EngineObject> renderGlobalEO;
	std::vector<std::shared_ptr<EngineObject>> renderLightEOs;
	std::vector<std::shared_ptr<EngineObject>> renderUnitEOs;

	// logic
	std::shared_ptr<EngineObject> logicMainCameraEO;
	std::vector<std::shared_ptr<EngineObject>> logicMoveEOs;
	std::vector<std::shared_ptr<EngineObject>> logicHitEOs;

	// all-engineObject
	std::vector<std::shared_ptr<EngineObject>> allEOs;
	std::unordered_map<std::string, std::shared_ptr<EngineObject>> allEOMap;

	void AddEO(std::shared_ptr<EngineObject> eo)
	{
		allEOs.emplace_back(eo);
		allEOMap.emplace(eo->name, eo);
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
		Editor::TestLogic::CreateMainCamera(this);
		Editor::TestLogic::CreateLight(this);
		Editor::TestLogic::CreateSkybox(this);
		Editor::TestLogic::CreateModel(this);
		Editor::TestLogic::CreateAxis(this);
	}

	void Update()
	{
		Render::System::Update(this);
		Logic::System::Update(this);
		Editor::System::Update(this);
	}

	void Destroy()
	{
		Render::System::Destroy(this);
		Logic::System::Destroy(this);
	}
};
