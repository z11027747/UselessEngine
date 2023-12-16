
#include "render/comp.h"
#include "context.h"

void RenderSystem::CreateGlobal(Context* context) {

	auto& renderEO = context->renderEO;

	renderEO = std::make_shared<EngineObject>();

	auto renderGlobal = std::make_shared<RenderGlobal>();
#ifndef NDEBUG
	renderGlobal->enableValidationLayer = true;
#else
	renderGlobal->enableValidationLayer = false;
#endif
	renderEO->AddComponent<RenderGlobal>(renderGlobal);
}