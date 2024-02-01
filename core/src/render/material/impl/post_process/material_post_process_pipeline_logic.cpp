
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_post_process_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialPostProcessPipelineLogic::SetViewport(Context *context,
															std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &globalEO = context->renderGlobalEO;
		auto global = globalEO->GetComponent<Render::Global>();
		auto &currentExtent = global->surfaceCapabilities.currentExtent;

		auto &stageInfo = graphicsPipeline->stageInfo;

		auto &viewport = stageInfo.viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(currentExtent.width);
		viewport.height = static_cast<float>(currentExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
	}
	void MaterialPostProcessPipelineLogic::SetColorBlendStage(Context *context,
														std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
}