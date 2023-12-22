#pragma once

#include "render/pipeline/pipeline_comp.h"

class Context;

namespace Render {

	class PipelineSystem final {
	public:

		static void Create(Context*);
		static void CreateLayout(Context*);
		static void CreateShaderModules(Context*);
	};

}