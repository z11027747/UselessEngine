#pragma once

class Context;

namespace Editor
{
	class System final
	{
	public:
		static void Create(Context *);
		static void Update(Context *);
		static void Destroy(Context *);
	};
}