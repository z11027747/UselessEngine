#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
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
			DrawConsole(context);
			DrawProject(context);
		}

		static void DrawMenuBar(Context *);
		static void DrawHierachy(Context *);
		static void DrawInspector(Context *);
		static void DrawViewport(Context *);
		static void DrawConsole(Context *);
		static void DrawProject(Context *);

		// hierachy&inspector
		static std::shared_ptr<EngineObject> GetSelectEO();
		static void SetSelectEO(Context *, std::shared_ptr<EngineObject> eo);

		// viewport
		static bool IsInViewport(Context *);
		static void ToViewportNdcXY(Context *, float &, float &);

		// project
		static void GetDirectoryFiles(const std::string &, std::vector<std::string> &);
	};
}