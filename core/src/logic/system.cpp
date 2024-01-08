#pragma once

#include "logic/camera/camera_system.h"
#include "logic/transform/transform_system.h"
#include "context.h"
#include "engine_object.h"

class Context;

namespace Logic
{
	void System::Create(Context *context)
	{
	}
	void System::Update(Context *context)
	{
		TransformUpdateMSystem::Update(context);
		CameraUpdateVPSystem::Update(context);
		CameraMoveSystem::Update(context);
	}
	void System::Destroy(Context *context)
	{
	}
}