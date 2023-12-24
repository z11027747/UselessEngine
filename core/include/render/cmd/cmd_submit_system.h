
#include <vulkan/vulkan.h>
#include <vector>
#include <functional>

class Context;

namespace Render {

	class CmdSubmitSystem final {
	public:

		static void CreateSimple(Context*,
			std::function<void(VkCommandBuffer&)> doCmds);

		static void CmdSubmitSystem::RecordCmd(VkCommandBuffer&,
			std::function<void(VkCommandBuffer&)> doCmds
		);

		static void Update(Context*);

	};

}