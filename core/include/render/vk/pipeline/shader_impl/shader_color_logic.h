#pragma once

#include <vulkan/vulkan.h>
#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/unit/unit_comp.h"

class Context;
class EngineObject;

namespace Render
{
	class ShaderColorLogic final
	{
	public:
		static void CreateVertexAttrDescriptions(Context *, std::shared_ptr<GraphicsPipeline>);
		static void SetRasterizationCreateInfo(Context *, std::shared_ptr<GraphicsPipeline>);
	};
}