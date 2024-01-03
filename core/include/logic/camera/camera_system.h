#pragma once

#include <glm/glm.hpp>
#include "logic/transform/transform_logic.h"
#include "logic/camera/camera_logic.h"
#include "context.h"
#include "engine_object.h"

namespace Logic
{
	class CameraSystem final
	{
	public:
		static void Create(Context *context)
		{
			CameraLogic::CreateMain(context);
		}

		static void Update(Context *context)
		{
			auto &cameraEOs = context->logicCameraEOs;
			for (const auto &cameraEO : cameraEOs)
			{
				CameraLogic::UpdateView(cameraEO);
			}
		}

		static void Destroy(Context *context)
		{
			CameraLogic::DestroyMain(context);
		}
	};
}
