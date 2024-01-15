#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

const std::string Name_MainCamera = "MainCamera";
const std::string Name_DirectionLight = "DirectionLight";
const std::string Name_Skybox = "Skybox";
const std::string Name_Axis = "Axis";
const std::string Name_AxisX = "X";
const std::string Name_AxisY = "Y";
const std::string Name_AxisZ = "Z";

class EngineObject;

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

	void Create();
	void Update();
	void Destroy();

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
	std::vector<std::shared_ptr<EngineObject>> logicHitCheckEOs;

	// editor
	std::vector<std::shared_ptr<EngineObject>> editorAxisEOs;

	// all-engineObject
	std::vector<std::shared_ptr<EngineObject>> allEOs;
	std::unordered_map<std::string, std::shared_ptr<EngineObject>> allEOMap;

	void AddEO(std::shared_ptr<EngineObject>);
	std::shared_ptr<EngineObject> GetEO(const std::string &);
	void DestroyEO(std::shared_ptr<EngineObject>, bool = true);
	void DestroyAllEOs();

	// scene
	std::string newSceneName{""};
};
