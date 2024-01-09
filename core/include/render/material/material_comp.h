#pragma once

#include <memory>
#include <string>
#include "render/vk/pipeline/descriptor_comp.h"
#include "render/vk/image/image_comp.h"

namespace Render
{
    struct Material final
    {
        std::string pipelineName;
        std::string image0Name;
        std::shared_ptr<Image> image0;
        std::shared_ptr<Descriptor> descriptor;
		bool castShadow;
    };
}
