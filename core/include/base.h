#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>

class EngineComp {
public:
	uint32_t type;
};

class EngineObject final {
public:

	std::map<uint32_t, std::unique_ptr<EngineComp>> compMap;

	template <typename T>
	T& GetComp(uint32_t type) {
		auto it = compMap.find(type);
		if (it != compMap.end()) {
			return static_cast<T&>(*it->second);
		}

		throw std::runtime_error("Component not found.");
	}

	template <typename T>
	void AddComp(T&& comp) {
		auto compPtr = std::make_unique<T>(std::forward<T>(comp));
		compMap.insert({ compPtr->type, std::move(compPtr) });
	}

	void RemoveComp(uint32_t type) {
		auto it = compMap.find(type);
		if (it != compMap.end()) {
			compMap.erase(it);
		}
	}

	void RemoveAllComps() {
		compMap.clear();
	}
};