#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <typeindex>

class EngineObject final {
public:
	inline static uint32_t beginId = 0;

	EngineObject() {
		id = beginId++;
		std::cout << "EngineObject id:" << id << " create." << std::endl;
	}
	~EngineObject() {
		std::cout << "EngineObject id:" << id << " destroy." << std::endl;
	}

	uint32_t id;
	std::unordered_map<std::type_index, std::shared_ptr<void>> compMap;

	template <typename T>
	void AddComponent(std::shared_ptr<T> comp) {
		compMap[typeid(T)] = comp;
	}

	template <typename T>
	std::shared_ptr<T> GetComponent() const {
		auto it = compMap.find(typeid(T));
		if (it != compMap.end()) {
			return std::static_pointer_cast<T>(it->second);
		}

		return nullptr;
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