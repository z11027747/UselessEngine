
#include "render/light/light_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/material/material_logic.h"
#include "render/system.h"
#include "logic/move/move_comp.h"
#include "logic/rotate/rotate_logic.h"
#include "logic/scene/scene_logic.h"
#include "logic/system.h"
#include "editor/system.h"
#include "common/res_system.h"
#include "engine_object.hpp"
#include "engine_component.hpp"
#include "context.hpp"

void Context::Create()
{
    Render::System::Create(this);
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
    Render::System::Destroy(this);
    Logic::System::Destroy(this);
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