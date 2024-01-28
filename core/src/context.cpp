
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
#include "editor/imgui_logic.h"
#include "editor/json/component_json.h"
#include "common/res_system.h"
#include "engine_object.h"
#include "context.h"

void Context::Create()
{
    Render::System::Create(this);
    Logic::System::Create(this);
}

void Context::Update()
{
    // 切换场景
    //  TODO 先放这
    if (!newSceneName.empty())
    {
        DestroyAllEOs();

        auto lines = Common::ResSystem::ReadAllLines("resource/scene/" + newSceneName);
        for (const auto &line : lines)
        {
            std::string err;
            auto jObj = json11::Json::parse(line, err);
            auto eo = Editor::EOFromJson(this, jObj);
            AddEO(eo);
        }
        newSceneName.clear();
    }

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
    // auto &componentMap = eo->componentMap;
    // for (const auto &kv : componentMap)
    // {
    //     auto &type = kv.first;
    //     Common::Type::OnAdd(type, this, eo);
    // }

    allEOs.push_back(eo);
    allEOMap.emplace(eo->name, eo);
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
        Common::Type::OnRemove(type, this, eo);
    }
    eo->RemoveAllComponents();

    if (remove)
    {
        allEOs.erase(std::remove(allEOs.begin(), allEOs.end(), eo), allEOs.end());
        allEOMap.erase(allEOMap.find(eo->name));
    }
}

void Context::DestroyAllEOs()
{
    for (const auto &eo : allEOs)
    {
        DestroyEO(eo, false);
    }
    allEOs.clear();
    allEOMap.clear();
}