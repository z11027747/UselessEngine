#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <string>

class EngineObject final
{
public:
	inline static uint32_t beginId = 0;

	EngineObject()
	{
		id = beginId++;
		name = "";
		active = true;
	}

	uint32_t id;
	std::string name;
	bool active;

	std::unordered_map<std::type_index, std::shared_ptr<void>> componentMap;

	template <typename T>
	bool HasComponent() const
	{
		auto it = componentMap.find(typeid(T));
		return (it != componentMap.end());
	}

	template <typename T>
	void AddComponent(std::shared_ptr<T> comp)
	{
		componentMap[typeid(T)] = comp;
	}

	template <typename T>
	std::shared_ptr<T> GetComponent() const
	{
		auto it = componentMap.find(typeid(T));
		if (it != componentMap.end())
		{
			return std::static_pointer_cast<T>(it->second);
		}

		return nullptr;
	}

	template <typename T>
	void RemoveComponent()
	{
		auto it = componentMap.find(typeid(T));
		if (it != componentMap.end())
		{
			componentMap.erase(it);
		}
	}

	void RemoveAllComponents()
	{
		componentMap.clear();
	}
};