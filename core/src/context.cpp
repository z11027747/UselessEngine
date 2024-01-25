
#include "render/light/light_comp.h"
#include "render/mesh/mesh_comp.h"
#include "render/mesh/mesh_logic.h"
#include "render/material/material_logic.h"
#include "render/system.h"
#include "logic/move/move_comp.h"
#include "logic/system.h"
#include "editor/system.h"
#include "editor/test_logic.h"
#include "editor/json/component_json.h"
#include "editor/json/component_json_mapping.h"
#include "common/res_system.h"
#include "engine_object.h"
#include "context.h"

void Context::Create()
{
    Render::System::Create(this);
    Logic::System::Create(this);

    // Test
    Editor::TestLogic::CreateMainCamera(this);
    Editor::TestLogic::CreateLight(this);
    Editor::TestLogic::CreateAxis(this);
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

void RemoveEoInVec(std::vector<std::shared_ptr<EngineObject>> &vec, std::shared_ptr<EngineObject> eo)
{
    vec.erase(std::remove(vec.begin(), vec.end(), eo), vec.end());
}

bool Context::CheckEO(std::shared_ptr<EngineObject> eo, bool add)
{
    if (eo->name == Name_MainCamera)
    {
        if (add)
            logicMainCameraEO = eo;
        else
            logicMainCameraEO = nullptr;
    }
    if (eo->name == Name_AxisX || eo->name == Name_AxisY || eo->name == Name_AxisZ)
    {
        if (add)
            editorAxisEOs.push_back(eo);
        else
            RemoveEoInVec(editorAxisEOs, eo);
    }

    if (eo->HasComponent<Render::DirectionLight>() ||
        eo->HasComponent<Render::PointLight>() ||
        eo->HasComponent<Render::SpotLight>())
    {
        if (add)
            renderLightEOs.push_back(eo);
        else
            RemoveEoInVec(renderLightEOs, eo);
    }
    if (eo->HasComponent<Render::Mesh>())
    {
        if (add)
            renderMeshEOs.push_back(eo);
        else
        {
            Render::MeshLogic::TryDestroyEO(this, eo);
            RemoveEoInVec(renderMeshEOs, eo);
        }
    }
    if (eo->HasComponent<Render::Material>())
    {
        if (add)
            renderMaterialEOs.push_back(eo);
        else
        {
            Render::MaterialLogic::TryDestroyEO(this, eo);
            RemoveEoInVec(renderMaterialEOs, eo);
        }
    }
    return true;
}

void Context::AddEO(std::shared_ptr<EngineObject> eo)
{
    CheckEO(eo, true);

    allEOs.push_back(eo);
    allEOMap.emplace(eo->name, eo);
}

std::shared_ptr<EngineObject> Context::GetEO(const std::string &name)
{
    return allEOMap[name];
}

void Context::DestroyEO(std::shared_ptr<EngineObject> eo, bool remove)
{
    CheckEO(eo, false);

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