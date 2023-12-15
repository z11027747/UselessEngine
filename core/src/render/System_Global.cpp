#include <vulkan/vulkan.h>
#include "render/comp.h"
#include "render/system.h"
#include "context.h"

void RenderSystem::CreateGlobal(Context* context) {

	auto& renderEO = context->renderEO;

	RenderGlobal renderGlobal = {};
#ifndef NDEBUG
	renderGlobal.enableValidationLayer = true;
#else
	renderGlobal.enableValidationLayer = false;
#endif
	renderEO.AddComponent<RenderGlobal>(std::move(renderGlobal));
}