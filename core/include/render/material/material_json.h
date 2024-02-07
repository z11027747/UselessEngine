
#pragma once

#include <memory>
#include <json/json11.hpp>

namespace Render
{
    class MaterialJson final
    {
    public:
        static std::shared_ptr<void> Parse(const json11::Json &);
        static json11::Json Serialize(std::shared_ptr<void>);
    };
}