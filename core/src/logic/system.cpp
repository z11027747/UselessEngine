#pragma once

#include "logic/camera/camera_system.h"
#include "logic/transform/transform_system.h"
#include "logic/move/move_system.h"
#include "logic/rotate/rotate_system.h"
#include "logic/scene/scene_system.h"
#include "logic/system.h"
#include "context.h"

class Context;

namespace Logic
{
	void System::Create(Context *context)
	{
		SceneInitSystem::Create(context);
	}
	void System::Update(Context *context)
	{
		TransformUpdateMSystem::Update(context);
		CameraUpdateVPSystem::Update(context);
		MoveFollowSystem::Update(context);
		RotateAroundSystem::Update(context);
	}
	void System::Destroy(Context *context)
	{
	}
}