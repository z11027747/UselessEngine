#pragma once

#include <string>
#include <memory>

class Context;
class EngineObject;

namespace Editor {

	class EngineObjectWrap {
	public:

		static void Add(Context*,
			const std::string&);

		static void Draw(Context*,
			std::shared_ptr<EngineObject>);

	};

}