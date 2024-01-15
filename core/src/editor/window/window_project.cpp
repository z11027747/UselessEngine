
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

    template <typename T>
    static void ComponentFromJson(Context *context,
                                  std::shared_ptr<EngineObject> eo, const json11::Json &j)
    {
        auto component = ComponentJson<T>::From(context, j);
        eo->AddComponent<T>(component);
    }

    static std::unordered_map<std::string, std::function<void(Context *, std::shared_ptr<EngineObject>, const json11::Json &)>> fromJsonFuncMap{
        {"Logic::Camera", ComponentFromJson<Logic::Camera>},
        {"Logic::Transform", ComponentFromJson<Logic::Transform>},
        {"Render::DirectionLight", ComponentFromJson<Render::DirectionLight>},
        {"Render::Material", ComponentFromJson<Render::Material>},
        {"Render::Mesh", ComponentFromJson<Render::Mesh>}};

    static void DrawDirectory(Context *context,
                              const std::string &directoryName, const std::string &directoryFullName)
    {
        if (ImGui::TreeNode(directoryName.c_str()))
        {
            if (directory2files.count(directoryFullName) == 0)
            {
                auto &directorySubs = directory2subs[directoryFullName];
                for (auto const &directorySubName : directorySubs)
                {
                    auto directorySubFullName = directoryFullName + "/" + directorySubName;
                    DrawDirectory(context,
                                  directorySubName, directorySubFullName);
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
                            auto lines = Common::ResSystem::ReadAllLines("resource/scene/" + fileName);
                            for (const auto &line : lines)
                            {
                                std::string err;
                                auto jObj = json11::Json::parse(line, err);

                                auto eo = std::make_shared<EngineObject>();
                                eo->name = jObj["name"].string_value();
                                eo->active = jObj["active"].bool_value();
                                eo->hideInHierarchy = jObj["hideInHierarchy"].bool_value();

                                auto &components = jObj["components"].array_items();
                                for (const auto &component : components)
                                {
                                    auto &componentType = component["type"].string_value();

                                    auto it = fromJsonFuncMap.find(componentType);
                                    if (it == fromJsonFuncMap.end())
                                        continue;

                                    fromJsonFuncMap[componentType](context, eo, component);
                                }

                                context->AddEO(eo);
                            }

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

    static char sceneName[16] = "test";

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

            ImGui::SetNextItemWidth(200.0f);
            ImGui::InputText("##sceneName", sceneName, IM_ARRAYSIZE(sceneName));
            ImGui::SameLine();
            if (ImGui::Button("SaveScene"))
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

                // Common::LogSystem::Info(sceneJson);
                Common::ResSystem::WriteFile("resource/scene/" + std::string(sceneName) + ".scene", sceneJson);
            }

            if (ImGui::IsMouseClicked(0))
            {
                selectFileName = "";
            }

            ImGui::SetNextItemOpen(true);
            DrawDirectory(context, resource, resource);
        }
        ImGui::End();
    }
}