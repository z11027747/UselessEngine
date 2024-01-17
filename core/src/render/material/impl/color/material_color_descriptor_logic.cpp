
#include <vulkan/vulkan.h>
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/material/impl/material_color_logic.h"
#include "context.h"

namespace Render
{
	void MaterialColorDescriptorLogic::CreateSetLayout(Context *context,
													   std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
	void MaterialColorDescriptorLogic::DestroySetLayout(Context *context,
														std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
	void MaterialColorDescriptorLogic::AllocateAndUpdate(Context *context,
														 std::shared_ptr<MaterialInstance> instance)
	{
	}
	void MaterialColorDescriptorLogic::Destroy(Context *context,
											   std::shared_ptr<MaterialInstance> instance)
	{
	}
}