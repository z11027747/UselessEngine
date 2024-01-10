#pragma once

#include <string>
#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"

class Context;

namespace Render
{
	class PipelineLayoutLogic final
	{
	public:
		static void Create(Context *, std::shared_ptr<GraphicsPipeline>);
		static void Destroy(Context *, std::shared_ptr<GraphicsPipeline>);
	};
}