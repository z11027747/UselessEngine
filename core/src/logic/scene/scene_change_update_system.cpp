
#include <string>
#include "logic/scene/scene_logic.h"
#include "logic/scene/scene_system.h"
#include "common/res_system.h"
#include "engine_object.hpp"
#include "context.hpp"

namespace Logic
{
    void SceneChangeUpdateSystem::Update(Context *context)
    {
        if (context->newSceneName.empty())
            return;

        auto newSceneName = context->newSceneName;
        context->newSceneName.clear();

        context->DestroyAllEOs();

        auto lines = Common::ResSystem::ReadAllLines("resource/scene/" + newSceneName);
        for (const auto &line : lines)
        {
            std::string err;
            auto j = json11::Json::parse(line, err);
            auto eo = SceneJsonLogic::ParseLine(context, j);
            context->AddEO(eo);
        }
    }
}
