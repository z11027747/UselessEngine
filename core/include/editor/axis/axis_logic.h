#pragma once

#include <memory>

class Context;
class EngineObject;

namespace Editor
{
	class AxisLogic final
	{
	public:
		static void OnAdd(Context *, std::shared_ptr<EngineObject>);
		static void OnRemove(Context *, std::shared_ptr<EngineObject>);
	};
}