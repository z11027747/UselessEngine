#pragma once

#include <string>
#include <memory>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pass/pass_comp.h"

class Context;

namespace Render
{
	class PipelineLayoutLogic final
	{
	public:
		static void Create(Context *, std::shared_ptr<GraphicsPipeline>, std::shared_ptr<Pass>);
		static void Destroy(Context *, std::shared_ptr<GraphicsPipeline>);
	};
}