#pragma once

#include <string>
#include <memory>

class Context;

namespace Editor
{
	template <typename T>
	class ComponentWrap
	{
	public:
		static void Draw(Context *, std::shared_ptr<T>, bool);
	};
}