
#include "render/vk/global/global_comp.h"
#include "render/vk/image/sampler_logic.h"
#include "render/render_pass/render_pass_logic.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Render
{
    void RenderPassLogic::DestroyDeferredDescriptor(Context *context)
    {
        auto &descriptor = descriptorMap[Define::Pass::Deferred];
        SamplerLogic::Destroy(context, descriptor->imageInfos[0].sampler);
    }
}
