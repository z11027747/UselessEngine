
#include <vulkan/vulkan.h>
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_shadow_logic.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialShadowDescriptorLogic::CreateSetLayout(Context *context,
														std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
	void MaterialShadowDescriptorLogic::DestroySetLayout(Context *context,
														 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
	void MaterialShadowDescriptorLogic::AllocateAndUpdate(Context *context,
														  std::shared_ptr<Material> material)
	{
	}
	void MaterialShadowDescriptorLogic::Destroy(Context *context,
												std::shared_ptr<Material> material)
	{
	}
}