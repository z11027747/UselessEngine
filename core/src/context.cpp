
#include "render/vk/global/global_logic.h"
#include "render/system.h"
#include "logic/system.h"
#include "editor/system.h"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

void Context::Create()
{
    Render::System::Create(this);
    Editor::System::Create(this);
    Logic::System::Create(this);
}

void Context::Update()
{
    Render::System::Update(this);
    Logic::System::Update(this);
    Editor::System::Update(this);
}

void Context::Destroy()
{
    // waitidle
    Render::LogicalDeviceLogic::WaitIdle(this);

    Logic::System::Destroy(this);
    Editor::System::Destroy(this);
    Render::System::Destroy(this);
}

void Context::AddEO(std::shared_ptr<EngineObject> eo)
{
    allEOs.push_back(eo);
    allEOMap[eo->name] = eo;
}

std::shared_ptr<EngineObject> Context::GetEO(const std::string &name)
{
    return allEOMap[name];
}

void Context::DestroyEO(std::shared_ptr<EngineObject> eo, bool remove)
{
    auto &componentMap = eo->componentMap;
    for (const auto &kv : componentMap)
    {
        auto &type = kv.first;
        EngineComponent::OnRemove(type, this, eo);
    }
    eo->_RemoveAllComponents();

    if (remove)
    {
        allEOs.erase(std::remove(allEOs.begin(), allEOs.end(), eo), allEOs.end());
        allEOMap.erase(allEOMap.find(eo->name));
    }
}

void Context::DestroyAllEOs()
{
    std::vector<std::shared_ptr<EngineObject>> dontDestroys = {};

    for (const auto &eo : allEOs)
    {
        if (eo->dontDestroy)
        {
            dontDestroys.push_back(eo);
            continue;
        }
        DestroyEO(eo, false);
    }
    allEOs.clear();
    allEOMap.clear();

    for (const auto &eo : dontDestroys)
    {
        allEOs.push_back(eo);
        allEOMap[eo->name] = eo;
    }
}