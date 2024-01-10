
#include <glm/glm.hpp>
#include <memory>
#include "logic/hit/hit_comp.h"

class Context;
class EngineObject;

namespace Logic
{
    class HitRayCheckLogic final
    {
    public:
        static std::shared_ptr<EngineObject> FromNdc(Context *, float, float);

        static bool Test(Context *,
                         HitRay &, std::shared_ptr<EngineObject>);
    };
}