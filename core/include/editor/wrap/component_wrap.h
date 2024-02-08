#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

class Context;

namespace Editor
{
    template <typename T>
    class ComponentWrap
    {
    public:
        static void Draw(Context *, std::shared_ptr<void>, bool);
    };

#define REGIST_WRAP(TComp) \
    ComponentWrapMapping::drawFuncMap[TComp::type] = ComponentWrap<TComp>::Draw;

    class ComponentWrapMapping final
    {
    public:
        inline static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<void>, bool)>> drawFuncMap{};

        inline static bool Has(const std::string &type)
        {
            auto it = drawFuncMap.find(type);
            return (it != drawFuncMap.end());
        }

        inline static void Draw(Context *context,
                                const std::string &type, std::shared_ptr<void> component, bool isInit)
        {
            if (Has(type))
                drawFuncMap[type](context, component, isInit);
        }
    };
}
