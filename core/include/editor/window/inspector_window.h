#pragma once

#include <memory>

class Context;
class EngineObject;

namespace Editor {

	class InspectorWindow final {
	public:

		static std::shared_ptr<EngineObject> selectEO;

		static void Draw(Context*);
	};

}