#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "render/vk/global/global_comp.h"
#include "render/light/light_comp.h"
#include "render/system.h"
#include "logic/input/input_comp.h"
#include "logic/camera/camera_comp.h"
#include "logic/system.h"
#include "editor/test.h"
#include "engine_object.h"

const std::string Name_MainCamera = "MainCamera";
const std::string Name_DirectionLight = "DirectionLight";
const std::string Name_Skybox = "Skybox";
const std::string Name_Axis = "Axis";

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

	// render
	std::shared_ptr<EngineObject> renderGlobalEO;
	std::vector<std::shared_ptr<EngineObject>> renderLightEOs;
	std::vector<std::shared_ptr<EngineObject>> renderUnitEOs;

	// logic
	std::shared_ptr<EngineObject> logicMainCameraEO;
	std::vector<std::shared_ptr<EngineObject>> logicMoveEOs;

	// all-engineObject
	std::vector<std::shared_ptr<EngineObject>> allEOs;
	std::unordered_map<std::string, std::shared_ptr<EngineObject>> allEOMap;

	void AddEO(std::shared_ptr<EngineObject> eo)
	{
		allEOs.emplace_back(eo);
		allEOMap.emplace(eo->name, eo);

		if (eo->HasComponent<Render::Unit>())
		{
			renderUnitEOs.push_back(eo);
		}
		if (eo->HasComponent<Render::DirectionLight>())
		{
			renderLightEOs.push_back(eo);
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
		Editor::Test::CreateModel(this);
		Editor::Test::CreateAxis(this);
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
};
