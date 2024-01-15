
#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <string>
#include <imgui/imgui.h>
#include <json/json11.hpp>
#include "editor/window.h"
#include "editor/json/component_json.h"
#include "editor/json/component_json_mapping.h"
#include "common/log_system.h"
#include "common/res_system.h"
#include "engine_object.h"
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
    static char sceneName[16] = "new.scene";

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

                        if (fileName.find(".scene") != std::string::npos)
                        {
                            if (ImGui::Button("Open Scene"))
                            {
                                context->newSceneName = fileName;
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::Separator();
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
                auto allEOSize = allEOs.size();
                for (auto i = 0; i < allEOSize; i++)
                {
                    auto &eo = allEOs[i];

                    auto eoJson = EOToJson(context, eo);
                    sceneJson += eoJson;

                    if (i != allEOSize - 1)
                        sceneJson += "\n";
                }

                // Common::LogSystem::Info(sceneJson);
                Common::ResSystem::WriteFile("resource/scene/" + std::string(sceneName), sceneJson);
            }

            if (ImGui::IsMouseClicked(0))
            {
                selectFileName.clear();
            }

            ImGui::SetNextItemOpen(true);
            DrawDirectory(context, resource, resource);
        }
        ImGui::End();
    }
}