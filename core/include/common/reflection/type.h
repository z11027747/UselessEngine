#pragma once

#include <string>
#include <memory>

class Context;
class EngineObject;

namespace Common
{
    class Type final
    {
    public:
        static std::shared_ptr<void> Create(const std::string &type);

        static void OnAdd(const std::string &,
                          Context *, std::shared_ptr<EngineObject>);
        static void OnRemove(const std::string &,
                             Context *, std::shared_ptr<EngineObject>);
    };
}