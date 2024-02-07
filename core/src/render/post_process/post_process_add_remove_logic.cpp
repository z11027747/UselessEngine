
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/pipeline/descriptor_set_layout_logic.h"
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/post_process/post_process_comp.h"
#include "render/post_process/post_process_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void PostProcessLogic::OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
    }
    void PostProcessLogic::OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
    }
}