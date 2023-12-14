
#include <iostream>
#include <vector>
#include <map>
#include "base.h"
#include "render/system.h"

class Context final {
public:

	//render
	EngineObject renderEO;

	void Create() {
		//render
		RenderSystem::CreateGlobal(this);
		RenderSystem::CreateVKInstance(this);
		//RenderSystem::CreateDebugCallback(this);
		RenderSystem::PickupPhysicalDevice(this);
	}

	void Update() {

	}

	void Destroy() {
		//RenderSystem::DestroyVKInstance(this);
	}

};
