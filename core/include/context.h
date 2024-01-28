#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "common/define.h"
#include "common/reflection/type.h"
#include "engine_object.h"

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
	std::vector<std::shared_ptr<EngineObject>> renderMeshEOs;
	std::vector<std::shared_ptr<EngineObject>> renderMaterialEOs;

	// logic
	std::shared_ptr<EngineObject> logicMainCameraEO;
	std::vector<std::shared_ptr<EngineObject>> logicMoveEOs;
	std::vector<std::shared_ptr<EngineObject>> logicRotateEOs;

	// editor
	std::vector<std::shared_ptr<EngineObject>> editorAxisEOs;

	// all-engineObject
	std::vector<std::shared_ptr<EngineObject>> allEOs;
	std::unordered_map<std::string, std::shared_ptr<EngineObject>> allEOMap;

	void AddEO(std::shared_ptr<EngineObject>);
	inline void RemoveEOInVector(std::shared_ptr<EngineObject> eo,
								 std::vector<std::shared_ptr<EngineObject>> &vector)
	{
		vector.erase(std::remove(vector.begin(), vector.end(), eo), vector.end());
	}
	std::shared_ptr<EngineObject> GetEO(const std::string &);
	void DestroyEO(std::shared_ptr<EngineObject>, bool = true);
	void DestroyAllEOs();

	template <typename T>
	void AddComponent(std::shared_ptr<EngineObject> eo, std::shared_ptr<T> comp)
	{
		eo->AddComponent<T>(comp);
		Common::Type::OnAdd(T::type, this, eo);
	}
	void AddComponent(std::shared_ptr<EngineObject> eo,
					  const std::string &type, std::shared_ptr<void> comp)
	{
		eo->AddComponent(type, comp);
		Common::Type::OnAdd(type, this, eo);
	}

	template <typename T>
	void RemoveComponent(std::shared_ptr<EngineObject> eo)
	{
		eo->RemoveComponent<T>();
		Common::Type::OnRemove(T::type, this, eo);
	}
	void AddComponent(std::shared_ptr<EngineObject> eo,
					  const std::string &type)
	{
		eo->RemoveComponent(type);
		Common::Type::OnRemove(type, this, eo);
	}

	// scene
	std::string newSceneName{""};
};
