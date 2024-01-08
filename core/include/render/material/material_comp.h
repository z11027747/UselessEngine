#pragma once

#include <memory>
#include <string>
#include "render/vk/pipeline/pipeline_comp.h"

namespace Render
{
    struct Material final
    {
        std::string pipelineName;
        std::shared_ptr<Descriptor> descriptor;
    };
}
