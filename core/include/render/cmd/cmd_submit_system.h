
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
			std::function<void(VkCommandBuffer&)> doCmds,
			VkSemaphore& waitSemaphore, VkSemaphore& signalSemaphore
		);

		//记录命令
		static VkCommandBuffer RecordCmd(Context*,
			std::function<void(VkCommandBuffer&)>);

		//统一提交命令
		static void Update(Context*);

		//统一提交异步信号命令
		static void UpdateSemaphore(Context*);

	};

}