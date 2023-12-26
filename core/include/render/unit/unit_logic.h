#pragma once

#include <vector>
#include <memory>
#include "render/unit/unit_comp.h";

class Context;

namespace Render {

	class UnitLogic final {
	public:

		static void SetVertices(Context*,
			std::shared_ptr<Unit>,
			std::vector<Vertex>&);

		static void SetIndices(Context*,
			std::shared_ptr<Unit>,
			std::vector<uint16_t>&);

		static void UpdateBuffers(Context*,
			std::shared_ptr<Unit>);

		static void UpdateVertexBuffer(Context*,
			std::shared_ptr<Unit>);

		static void UpdateIndexBuffer(Context*,
			std::shared_ptr<Unit>);

		static void Destroy(Context*);

	};

}