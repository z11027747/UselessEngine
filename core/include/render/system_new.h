#pragma once

class Context;

namespace Render {

	class System final {
	public:

		static void OnCreate(Context*);
		static void OnUpdate(Context*);
		static void OnDestroy(Context*);

	};

}