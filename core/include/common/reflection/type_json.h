#pragma once

#include <string>
#include <memory>
#include <json/json11.hpp>

namespace Common
{
    class TypeJson final
    {
    public:
        static std::shared_ptr<void> From(const std::string &type, const json11::Json &j);
        static json11::Json To(const std::string &type, std::shared_ptr<void> component);
    };

    template <typename T>
    class ComponentJson
    {
    public:
        static std::shared_ptr<void> From(const json11::Json &);
        static json11::Json To(std::shared_ptr<void>);
    };
}