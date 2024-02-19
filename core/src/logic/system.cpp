#pragma once

#include "logic/camera/camera_system.h"
#include "logic/hit/hit_system.h"
#include "logic/transform/transform_system.h"
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
		TransformCreateSystem::Create(context);
		MoveCreateSystem::Create(context);
		RotateCreateSystem::Create(context);
		ScaleCreateSystem::Create(context);
		SceneCreateSystem::Create(context);
	}
	void System::Update(Context *context)
	{
		CameraUpdateVPSystem::Update(context);
		MoveFollowSystem::Update(context);
		TransformUpdateMSystem::Update(context);
		RotateAroundSystem::Update(context);
		ScaleKeepSystem::Update(context);
		SceneChangeSystem::Update(context);
	}
	void System::Destroy(Context *context)
	{
	}
}