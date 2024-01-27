#pragma once

#include <string>
#include <memory>
#include <json/json11.hpp>

class Context;

namespace Editor
{
    template <typename T>
    class ComponentJson
    {
    public:
        static std::shared_ptr<void> From(const json11::Json &);
        static json11::Json To(std::shared_ptr<void>);
    };
}