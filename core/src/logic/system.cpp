#pragma once

#include "logic/camera/camera_system.h"
#include "logic/hit/hit_system.h"
#include "logic/transform/transform_system.h"
#include "logic/move/move_system.h"
#include "logic/rotate/rotate_system.h"
#include "logic/scene/scene_system.h"
#include "logic/system.h"
#include "context.hpp"

class Context;

namespace Logic
{
	void System::Create(Context *context)
	{
		CameraCreateSystem::Create(context);
		HitCreateSystem::Create(context);
		MoveCreateSystem::Create(context);
		RotateCreateSystem::Create(context);
		TransformCreateSystem::Create(context);
		SceneCreateSystem::Create(context);
	}
	void System::Update(Context *context)
	{
		TransformUpdateMSystem::Update(context);
		CameraUpdateVPSystem::Update(context);
		MoveFollowSystem::Update(context);
		RotateAroundSystem::Update(context);
		SceneChangeSystem::Update(context);
	}
	void System::Destroy(Context *context)
	{
	}
}