#pragma once

#include "logic/camera/camera_system.h"
#include "context.h"
#include "engine_object.h"

class Context;

namespace Logic {

	void System::Create(Context* context){

		CameraSystem::Create(context);
	}

	void System::Update(Context* context) {
	
		CameraSystem::UpdateView(context);
	}


	void System::Destroy(Context* context) {
	
	}

}