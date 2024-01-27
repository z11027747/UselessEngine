#pragma once

#include <string>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pass/pass_comp.h"

class Context;

namespace Render
{
	class PipelineLogic final
	{
	public:
		static void Create(Context *, const std::string &, std::shared_ptr<Pass>);
		static void Destroy(Context *, std::shared_ptr<GraphicsPipeline>);
		static void DestroyAll(Context *);

		static void CreateShaderStage(Context *, std::shared_ptr<GraphicsPipeline>);
		static void CreateVertexInputStage(Context *, std::shared_ptr<GraphicsPipeline>);
		static void CreateInputAssemblyStage(Context *, std::shared_ptr<GraphicsPipeline>);
		static void CreateViewportStage(Context *, std::shared_ptr<GraphicsPipeline>);
		static void CreateRasterizationStage(Context *, std::shared_ptr<GraphicsPipeline>);
		static void CreateMultisampleStage(Context *, std::shared_ptr<GraphicsPipeline>, std::shared_ptr<Pass>);
		static void CreateDepthStencilStage(Context *, std::shared_ptr<GraphicsPipeline>);
		static void CreateColorBlendStage(Context *, std::shared_ptr<GraphicsPipeline>);

		static void DestroyShaderStage(Context *, std::shared_ptr<GraphicsPipeline>);
	};
}