#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <stdint.h>

class Context;
class EngineObject;

namespace Editor
{
	class Window final
	{
	public:
		static void Draw(Context *context)
		{
			DrawMenuBar(context);
			DrawHierachy(context);
			DrawInspector(context);
			DrawViewport(context);
			DrawProject(context);
		}

		static void DrawMenuBar(Context *);
		static void DrawHierachy(Context *);
		static void DrawInspector(Context *);
		static void DrawProject(Context *);

		static bool IsInViewport(Context *);
		static void ToViewportNdcXY(Context *, float &, float &);
		static void DrawViewport(Context *);

		static std::shared_ptr<EngineObject> selectEO;
		static void SetSelectEO(Context *, std::shared_ptr<EngineObject> eo);
	};
}