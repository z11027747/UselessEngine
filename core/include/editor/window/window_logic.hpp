#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <imgui/imgui.h>

class Context;
class EngineObject;

namespace Editor
{
    class WindowLogic final
    {
    public:
        // hierachy&inspector
        static std::shared_ptr<EngineObject> selectEO;

        static std::shared_ptr<EngineObject> GetSelectEO();
        static void SetSelectEO(Context *, std::shared_ptr<EngineObject> eo);

        // viewport
        static bool viewportFocuesd;
        static ImVec2 viewportPos;
        static ImVec2 viewportSize;

        static bool IsInViewport(Context *);
        static void ToViewportNdcXY(Context *, float &, float &);

        // project
        static std::unordered_map<std::string, std::vector<std::string>> directory2subs;
        static std::unordered_map<std::string, std::vector<std::string>> directory2files;

        static void GetDirectoryFiles(const std::string &, std::vector<std::string> &);
        static void RefreshDirectory(const std::string &);
    };
}