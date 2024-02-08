
#include <iostream>
#include <filesystem>
#include <imgui/imgui.h>
#include "logic/move/move_logic.h"
#include "editor/wrap/component_wrap.h"
#include "editor/window/window_system.hpp"
#include "editor/window/window_logic.hpp"
#include "engine_component.hpp"
#include "engine_object.hpp"
#include "context.hpp"

namespace Editor
{
    std::shared_ptr<EngineObject> WindowLogic::selectEO = nullptr;

    std::shared_ptr<EngineObject> WindowLogic::GetSelectEO()
    {
        return selectEO;
    }

    void WindowLogic::SetSelectEO(Context *context, std::shared_ptr<EngineObject> eo)
    {
        selectEO = eo;

        auto validEO = (eo->name != Define::EOName::MainCamera &&
                        eo->name != Define::EOName::Skybox &&
                        eo->name != Define::EOName::AxisX &&
                        eo->name != Define::EOName::AxisY &&
                        eo->name != Define::EOName::AxisZ);

        auto axisEO = context->GetEO(Define::EOName::Axis);
        axisEO->active = validEO;

        if (validEO)
            Logic::MoveLogic::BeginFollow(context,
                                          axisEO,
                                          eo, glm::vec3(0.0f));

        auto &componentMap = selectEO->componentMap;
        for (const auto &kv : componentMap)
        {
            auto type = kv.first;
            auto &component = kv.second;
            ComponentWrapMapping::Draw(context, type, component, true);
        }
    }

    bool WindowLogic::viewportFocuesd = false;
    ImVec2 WindowLogic::viewportPos = ImVec2(0.0, 0.0);
    ImVec2 WindowLogic::viewportSize = ImVec2(0.0, 0.0);

    bool WindowLogic::IsInViewport(Context *context)
    {
        return viewportFocuesd;
    }

    void WindowLogic::ToViewportNdcXY(Context *context,
                                      float &x, float &y)
    {
        auto &style = ImGui::GetStyle();

        // viewport space
        x = x - viewportPos.x - style.WindowPadding.x;
        y = (viewportPos.y + viewportPos.y) - y - style.WindowPadding.y;

        // ndc space
        x = 2.0f * x / viewportSize.x - 1.0f;
        y = 2.0f * y / (viewportSize.y - viewportPos.y) - 1.0f;
    }

    std::unordered_map<std::string, std::vector<std::string>> WindowLogic::directory2subs = {};
    std::unordered_map<std::string, std::vector<std::string>> WindowLogic::directory2files = {};

    void WindowLogic::GetDirectoryFiles(const std::string &directoryName, std::vector<std::string> &fileNames)
    {
        auto &subs = directory2subs[directoryName];
        for (auto &sub : subs)
        {
            auto &subFiles = directory2files[directoryName + "/" + sub];
            for (auto &subFile : subFiles)
            {
                auto subFileName = (directoryName + "/" + sub + "/" + subFile);
                fileNames.push_back(subFileName);
            }
        }

        auto &files = directory2files[directoryName];
        for (auto &file : files)
        {
            auto fileName = (directoryName + "/" + file);
            fileNames.push_back(fileName);
        }
    }

    void WindowLogic::RefreshDirectory(const std::string &directoryName)
    {
        for (const auto &entry : std::filesystem::directory_iterator(directoryName))
        {
            if (std::filesystem::is_directory(entry.status()))
            {
                auto chilDirectoryName = entry.path().filename().string();
                directory2subs[directoryName].push_back(chilDirectoryName);
                RefreshDirectory(directoryName + "/" + chilDirectoryName);
            }

            if (std::filesystem::is_regular_file(entry.status()))
            {
                auto fileName = entry.path().filename().string();
                directory2files[directoryName].push_back(fileName);
            }
        }
    }
}