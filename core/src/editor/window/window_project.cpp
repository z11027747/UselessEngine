
#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <imgui/imgui.h>
#include <json/json11.hpp>
#include "render/light/light_comp.h"
#include "render/material/material_comp.h"
#include "render/mesh/mesh_comp.h"
#include "logic/camera/camera_comp.h"
#include "editor/window.h"
#include "editor/json/component_json.h"
#include "common/log_system.h"
#include "common/res_system.h"
#include "context.h"

namespace Editor
{
    const std::string resource = "resource";

    static bool isInited = false;
    static std::unordered_map<std::string, std::vector<std::string>> directory2subs;  // resource -> obj/shader/spv/texture
    static std::unordered_map<std::string, std::vector<std::string>> directory2files; // resource/obj -> xxxx.obj

    static void RefreshDirectory(const std::string &directoryName)
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

    static std::string selectFileName = "";

    static void DrawDirectory(const std::string &directoryName, const std::string &directoryFullName)
    {
        if (ImGui::TreeNode(directoryName.c_str()))
        {
            if (directory2files.count(directoryFullName) == 0)
            {
                auto &directorySubs = directory2subs[directoryFullName];
                for (auto const &directorySubName : directorySubs)
                {
                    auto directorySubFullName = directoryFullName + "/" + directorySubName;
                    DrawDirectory(directorySubName, directorySubFullName);
                }
            }
            else
            {
                auto &fileNames = directory2files[directoryFullName];
                for (auto const &fileName : fileNames)
                {
                    auto id = fileName.c_str();
                    ImGui::PushID(id);
                    ImGui::Indent();

                    if (ImGui::Selectable(id, (selectFileName == fileName)))
                    {
                        selectFileName = fileName;
                        ImGui::OpenPopup(id);
                    }

                    if (ImGui::BeginPopup(id))
                    {
                        ImGui::SeparatorText(id);

                        if (ImGui::Button("Open (Only For Scene)"))
                        {
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("Reimport"))
                        {
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("Show in Explorer"))
                        {
                            ImGui::CloseCurrentPopup();
                        }
                        if (ImGui::Button("Copy Name"))
                        {
                            ImGui::SetClipboardText((directoryFullName + fileName).c_str());
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
                    }

                    ImGui::Unindent();
                    ImGui::PopID();
                }
            }

            ImGui::TreePop();
        }
    }

    template <typename T>
    static json11::Json ComponentToJson(Context *context, std::shared_ptr<void> component)
    {
        return ComponentJson<T>::To(context, std::static_pointer_cast<T>(component));
    }

    static std::unordered_map<std::type_index, std::function<json11::Json(Context *, std::shared_ptr<void>)>> toJsonFuncMap{
        {typeid(Logic::Camera), ComponentToJson<Logic::Camera>},
        {typeid(Logic::Transform), ComponentToJson<Logic::Transform>},
        {typeid(Render::DirectionLight), ComponentToJson<Render::DirectionLight>},
        {typeid(Render::Material), ComponentToJson<Render::Material>},
        {typeid(Render::Mesh), ComponentToJson<Render::Mesh>}};

    void Window::DrawProject(Context *context)
    {
        if (ImGui::Begin("Project", NULL))
        {
            if (!isInited)
            {
                RefreshDirectory(resource);
                isInited = true;
            }

            if (ImGui::Button("Refresh"))
            {
            }
            ImGui::SameLine();

            if (ImGui::Button("Save All"))
            {
                std::string sceneJson = "";

                auto &allEOs = context->allEOs;
                for (const auto &eo : allEOs)
                {
                    json11::Json::array componentJObjArr = {};
                    auto &componentMap = eo->componentMap;
                    for (const auto &kv : componentMap)
                    {
                        auto typeId = kv.first;
                        auto &component = kv.second;

                        auto it = toJsonFuncMap.find(typeId);
                        if (it == toJsonFuncMap.end())
                            continue;

                        auto componentJObj = toJsonFuncMap[typeId](context, component);
                        componentJObjArr.push_back(componentJObj);
                    }

                    json11::Json::object eoJObj = {
                        {"name", eo->name},
                        {"active", eo->active},
                        {"hideInHierarchy", eo->hideInHierarchy},
                        {"components", componentJObjArr}};

                    sceneJson += json11::Json(eoJObj).dump();
                    sceneJson += "\n";
                }

                Common::LogSystem::Info(sceneJson);
                Common::ResSystem::WriteFile("test.scene", sceneJson);
            }

            if (ImGui::IsMouseClicked(0))
            {
                selectFileName = "";
            }

            ImGui::SetNextItemOpen(true);
            DrawDirectory(resource, resource);
        }
        ImGui::End();
    }
}