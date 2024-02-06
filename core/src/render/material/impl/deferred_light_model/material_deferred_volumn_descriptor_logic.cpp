
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/pipeline/pipeline_comp.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/mesh/mesh_comp.h"
#include "render/material/impl/material_deferred_volumn_logic.h"
#include "common/define.h"
#include "engine_object.h"
#include "context.h"

namespace Render
{
	void MaterialDeferredVolumnDescriptorLogic::CreateSetLayout(Context *context,
																	 std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	{
	}
}