#pragma once

#include <unordered_set>

class Context;
class EngineObject;

namespace Logic
{
    struct Input final
    {
        std::unordered_set<int> downSet;
    };
}