#pragma once

#include <vector>
#include <array>
#include <memory>
#include <string>
#include "render/vk/pipeline/pipeline_comp.h";
#include "render/unit/unit_comp.h";

class Context;

namespace Render {

	class UnitLogic final {
	public:

		static void Destroy(Context*);

		static void SetPipelineName(Context*,
			std::shared_ptr<Unit>,
			std::string);

		static void SetVertices(Context*,
			std::shared_ptr<Unit>,
			std::vector<Vertex>&);

		static void SetIndices(Context*,
			std::shared_ptr<Unit>,
			std::vector<uint16_t>&);

		static void SetTexture(Context*,
			std::shared_ptr<Unit>,
			std::string);

		static void SetCubeTexture(Context*,
			std::shared_ptr<Unit>,
			std::array<std::string, 6>);

	};

}