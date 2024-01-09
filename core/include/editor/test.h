#pragma once

#include <render/unit/unit_comp.h>

class Context;

namespace Editor
{
	class Test final
	{
	public:
		static void CreateMainCamera(Context *);
		static void CreateLight(Context *);
		static void CreateSkybox(Context *);
		static void CreateModel(Context *);
		static void CreateAxis(Context *);
	};
}
