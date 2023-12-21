
#include <vulkan/vulkan.h>
#include <vector>
#include <functional>

class Context;

namespace Render {

	class CmdSubmitSystem final {
	public:

		//同步命令
		static void Create(Context*,
			std::function<void(VkCommandBuffer&)> doCmds);

		//异步信号命令
		static void CreateAsync(Context*,
			VkCommandBuffer& buffer,
			VkSemaphore& wait,
			VkSemaphore& signal
		);

		//统一提交命令
		static void Update(Context*);

	};

}