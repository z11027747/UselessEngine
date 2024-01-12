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
        static json11::Json To(Context *, std::shared_ptr<T>);
    };
}