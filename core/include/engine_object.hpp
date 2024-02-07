#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <functional>

class EngineObject final
{
public:
	inline static int beginId{0};

	EngineObject() : id(beginId++) {}

	int id;
	std::string name{""};
	bool active{true};

	// TODO Bits
	bool hideInHierarchy{false};
	bool save{true};
	bool dontDestroy{false};

	std::map<std::string, std::shared_ptr<void>> componentMap{};

	template <typename T>
	inline bool HasComponent() const
	{
		auto it = componentMap.find(T::type);
		return (it != componentMap.end());
	}

	template <typename T>
	inline void AddComponent(std::shared_ptr<T> comp)
	{
		AddComponent(T::type, comp);
	}
	inline void AddComponent(const std::string &type, std::shared_ptr<void> comp)
	{
		componentMap[type] = comp;
	}

	template <typename T>
	inline std::shared_ptr<T> GetComponent() const
	{
		auto it = componentMap.find(T::type);
		if (it != componentMap.end())
		{
			return std::static_pointer_cast<T>(it->second);
		}

		return nullptr;
	}

	template <typename T>
	inline void RemoveComponent()
	{
		RemoveComponent(T::type);
	}
	inline void RemoveComponent(const std::string &type)
	{
		auto it = componentMap.find(type);
		if (it != componentMap.end())
		{
			componentMap.erase(it);
		}
	}

	inline void RemoveAllComponents()
	{
		componentMap.clear();
	}
};