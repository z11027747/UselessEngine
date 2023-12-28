#pragma once

class Context;

namespace Render {

	class System final {
	public:

		static void Create(Context*);
		static void Update(Context*);
		static void Destroy(Context*);

	};

}