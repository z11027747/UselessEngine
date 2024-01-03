#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"

class Context;
class EngineObject;

namespace Render
{
	class ShaderShadowLogic final
	{
	public:
		static void CreateVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
	};
}