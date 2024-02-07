#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <json/json11.hpp>

class Context;
class EngineObject;

#define REGIST_CREATE(TComp) \
    EngineComponent::createFuncMap[TComp::type] = []() { return std::make_shared<TComp>(); }

#define REGIST_ADD_REMOVE(TComp, TLogic)                                                                                                 \
    EngineComponent::onAddFuncMap[TComp::type] = [](Context *context, std::shared_ptr<EngineObject> eo) { TLogic::OnAdd(context, eo); }; \
    EngineComponent::onRemoveFuncMap[TComp::type] = [](Context *context, std::shared_ptr<EngineObject> eo) { TLogic::OnRemove(context, eo); };

#define REGIST_JSON(TComp, TJson)                                                                       \
    EngineComponent::jsonParseMap[TComp::type] = [](const json11::Json &j) { return TJson::Parse(j); }; \
    EngineComponent::jsonSerializeMap[TComp::type] = [](std::shared_ptr<void> comp) { return TJson::Serialize(comp); };

class EngineComponent final
{
public:
    // create
    inline static std::unordered_map<std::string, std::function<std::shared_ptr<void>()>> createFuncMap{};

    inline static std::shared_ptr<void> Create(const std::string &type)
    {
        return createFuncMap[type]();
    }
    template <typename T>
    inline static std::shared_ptr<T> Create()
    {
        return std::static_pointer_cast<T>(Create(T::type));
    }

    // on add/remove
    inline static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<EngineObject>)>> onAddFuncMap{};
    inline static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<EngineObject>)>> onRemoveFuncMap{};

    inline static void OnAdd(const std::string &type, Context *context, std::shared_ptr<EngineObject> eo)
    {
        auto it = onAddFuncMap.find(type);
        if (it != onAddFuncMap.end())
            onAddFuncMap[type](context, eo);
    }
    template <typename T>
    inline static void OnAdd(Context *context, std::shared_ptr<EngineObject> eo)
    {
        OnAdd(T::type, context, eo);
    }
    inline static void OnRemove(const std::string &type, Context *context, std::shared_ptr<EngineObject> eo)
    {
        auto it = onRemoveFuncMap.find(type);
        if (it != onRemoveFuncMap.end())
            onRemoveFuncMap[type](context, eo);
    }
    template <typename T>
    inline static void OnRemove(Context *context, std::shared_ptr<EngineObject> eo)
    {
        OnRemove(T::type, context, eo);
    }

    // json
    inline static std::unordered_map<std::string, std::function<std::shared_ptr<void>(const json11::Json &)>> jsonParseMap{};
    inline static std::unordered_map<std::string, std::function<json11::Json(std::shared_ptr<void>)>> jsonSerializeMap{};

    inline static std::shared_ptr<void> JsonParse(const std::string &type, const json11::Json &j)
    {
        auto it = jsonParseMap.find(type);
        if (it != jsonParseMap.end())
            return jsonParseMap[type](j);

        return nullptr;
    }
    template <typename T>
    inline static std::shared_ptr<T> JsonParse(const json11::Json &j)
    {
        return std::static_pointer_cast<T>(JsonParse(T::type, j));
    }
    inline static json11::Json JsonSerialize(const std::string &type, std::shared_ptr<void> component)
    {
        auto it = jsonSerializeMap.find(type);
        if (it != jsonSerializeMap.end())
            return jsonSerializeMap[type](component);

        return {};
    }
    template <typename T>
    inline static json11::Json JsonSerialize(std::shared_ptr<T> component)
    {
        return JsonSerialize(T::type, component);
    }
};