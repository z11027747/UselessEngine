#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>

struct EngineComp {
};

class EngineObject final {
public:

	std::unordered_map<std::type_index, std::unique_ptr<EngineComp>> compMap;

	template <typename T>
	void AddComponent(T&& comp) {
		auto compPtr = std::make_unique<T>(std::forward<T>(comp));
		compMap[typeid(T)] = std::move(compPtr);
	}

	template <typename T>
	T& GetComponent() const {
		auto it = compMap.find(typeid(T));
		if (it != compMap.end()) {
			return static_cast<T&>(*it->second);
		}

		throw std::runtime_error("Component not found.");
	}

	template <typename T>
	void RemoveComponent() {
		auto it = compMap.find(typeid(T));
		if (it != compMap.end()) {
			compMap.erase(it);
		}
	}

	void RemoveAllComponents() {
		compMap.clear();
	}
};