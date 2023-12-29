#pragma once

class Context;
class EngineObject;

namespace Editor {

	class Window final {
	public:

		static void Draw(Context* context) {
			DrawMenuBar(context);
			DrawHierachy(context);
			DrawInspector(context);
		}

		static void DrawMenuBar(Context*);
		static void DrawHierachy(Context*);
		static void DrawInspector(Context*);

		static std::shared_ptr<EngineObject> selectEO;
	};

}