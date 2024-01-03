#pragma once

#include <memory>
#include "logic/camera/camera_comp.h"

class Context;
class EngineObject;

namespace Logic
{
	class CameraLogic final
	{
	public:
		static void CreateMain(Context *);
		static void DestroyAll(Context *);

		static void UpdateView(std::shared_ptr<EngineObject>);
		static void UpdateProjection(std::shared_ptr<Camera>);
	};
}