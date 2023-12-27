
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/cmd/cmd_comp.h"
#include "render/vk/cmd/cmd_pool_logic.h"
#include "render/vk/cmd/cmd_submit_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/shader_logic.h";
#include "render/unit/unit_comp.h";
#include "render/unit/unit_logic.h";
#include "context.h"
#include "base.h"

namespace Render {

	void UnitLogic::Destroy(Context* context) {
		auto& renderUnitEOs = context->renderUnitEOs;
		for (const auto& renderUnitEO : renderUnitEOs) {
			auto unit = renderUnitEO->GetComponent<Render::Unit>();

			BufferLogic::Destroy(context,
				unit->vertexBuffer);

			BufferLogic::Destroy(context,
				unit->indexBuffer);

		}
		renderUnitEOs.clear();
	}

	void UnitLogic::SetVertices(Context* context,
		std::shared_ptr<Unit> unit,
		std::vector<Vertex>& vertices
	) {
		auto vertexSize = static_cast<VkDeviceSize>(sizeof(vertices[0]) * vertices.size());

		unit->vertices = vertices;
		unit->vertexBuffer = BufferLogic::Create(context,
			vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferLogic::SetValues(context,
			unit->vertexBuffer,
			unit->vertices);
	}

	void UnitLogic::SetIndices(Context* context,
		std::shared_ptr<Unit> unit,
		std::vector<uint16_t>& indices
	) {
		auto indexSize = static_cast<VkDeviceSize>(sizeof(indices[0]) * indices.size());

		unit->indices = indices;
		unit->indexBuffer = BufferLogic::Create(context,
			indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferLogic::SetValues(context,
			unit->indexBuffer,
			unit->indices);
	}


}