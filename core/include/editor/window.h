#pragma once

class Context;
class EngineObject;

namespace Editor
{
	class Window final
	{
	public:
		static void Draw(Context *context, uint32_t imageIndex)
		{
			DrawMenuBar(context);
			DrawHierachy(context);
			DrawInspector(context);
			DrawViewport(context, imageIndex);
			DrawProject(context);
		}

		static void DrawMenuBar(Context *);
		static void DrawHierachy(Context *);
		static void DrawInspector(Context *);
		static void DrawViewport(Context *, uint32_t);
		static void DrawProject(Context *);

		static std::shared_ptr<EngineObject> selectEO;
		static void SetSelectEO(Context *, std::shared_ptr<EngineObject> eo);
	};
}