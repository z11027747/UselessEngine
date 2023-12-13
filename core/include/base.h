#pragma once

#include <map>

class BaseComp {
public:
	uint32_t type;
};

class EngineObject final {
public:

	std::map<uint32_t, BaseComp> compMap;

	EngineObject() {

	}
	~EngineObject() {

	}

	BaseComp& GetComp(uint32_t type) {
		return compMap[type];
	}

	void AddComp(const BaseComp& comp) {
		compMap.insert({ comp.type, comp });
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