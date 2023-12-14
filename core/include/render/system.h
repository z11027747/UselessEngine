#pragma once

#include <vector>

namespace Render {

	class System final {
	public:

		//1.创建VkInstance
		//	a)负责收集每个应用程序的状态信息
		//	b)创建几乎可执行所有操作的逻辑设备

		static void CreateBaseInfo(void*);

	};

}