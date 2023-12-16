#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <stdexcept>

struct EngineComp {
	virtual ~EngineComp() {};
};

class EngineObject final {
public:

	std::unordered_map<std::type_index, std::shared_ptr<EngineComp>> compMap;

	EngineObject() = default;

	template <typename T>
	void AddComponent(std::shared_ptr<T>& comp) {
		compMap.emplace(typeid(T), comp);
	}

	template <typename T>
	std::shared_ptr<T> GetComponent() const {
		auto it = compMap.find(typeid(T));
		if (it != compMap.end()) {
			return std::static_pointer_cast<T>(it->second);
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