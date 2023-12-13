#include "render/comp.h"
#include "render/system.h"
#include "context.h"

namespace Render {


	void System::CreateBaseInfo(void* ctx) {

		auto* context = (Context*)ctx;
		auto& renderEO = context->renderEO;

		BaseInfo baseInfo;
		baseInfo.type = Type::eBaseInfo;
		baseInfo.instance = nullptr;

		renderEO.AddComp(baseInfo);

	}



}