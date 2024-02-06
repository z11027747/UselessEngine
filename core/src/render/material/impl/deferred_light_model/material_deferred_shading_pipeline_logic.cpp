
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_deferred_shading_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
    void MaterialDeferredShadingPipelineLogic::SetRasterizationCreateInfo(Context *context,
                                                                          std::shared_ptr<GraphicsPipeline> graphicsPipeline)
    {
        auto &stageInfo = graphicsPipeline->stageInfo;

        auto &rasterizationStateCreateInfo = stageInfo.rasterizationStateCreateInfo;
        rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    }
	void MaterialDeferredShadingPipelineLogic::SetDepthStencilCreateInfo(Context *context,
															   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
		auto &stageInfo = graphicsPipeline->stageInfo;

	}
}