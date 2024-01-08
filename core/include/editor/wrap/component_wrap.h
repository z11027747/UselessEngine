#pragma once

#include <string>
#include <memory>
#include "logic/camera/camera_comp.h"
#include "logic/transform/transform_comp.h"
#include "render/light/light_comp.h"
#include "render/unit/unit_comp.h"

class Context;

namespace Editor
{
	template <typename T>
	class ComponentWrap
	{
	public:
		static void Draw(Context *, std::shared_ptr<T>, bool);
	};

	template <>
	class ComponentWrap<Logic::Camera>
	{
	public:
		static void Draw(Context *, std::shared_ptr<Logic::Camera>, bool);
	};

	template <>
	class ComponentWrap<Logic::Transform>
	{
	public:
		static void Draw(Context *, std::shared_ptr<Logic::Transform>, bool);
	};

	template <>
	class ComponentWrap<Render::DirectionLight>
	{
	public:
		static void Draw(Context *, std::shared_ptr<Render::DirectionLight>, bool);
	};

	template <>
	class ComponentWrap<Render::Unit>
	{
	public:
		static void Draw(Context *, std::shared_ptr<Render::Unit>, bool);
	};

}