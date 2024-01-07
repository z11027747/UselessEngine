
#include <vulkan/vulkan.h>
#include "render/vk/global/global_comp.h"
#include "render/vk/cmd/cmd_logic.h"
#include "render/vk/buffer/buffer_comp.h"
#include "render/vk/buffer/buffer_logic.h"
#include "render/vk/pipeline/shader_logic.h"
#include "render/vk/pipeline/descriptor_set_logic.h"
#include "render/vk/image/image_comp.h"
#include "render/vk/image/image_logic.h"
#include "render/vk/image/sampler_logic.h"
#include "render/unit/unit_comp.h"
#include "render/unit/unit_logic.h"
#include "common/res_system.h"
#include "context.h"
#include "engine_object.h"

namespace Render
{
	void UnitLogic::Destroy(Context *context)
	{
		auto &unitEOs = context->renderUnitEOs;
		for (const auto &unitEO : unitEOs)
		{
			auto unit = unitEO->GetComponent<Render::Unit>();

			BufferLogic::Destroy(context, unit->vertexBuffer);
			BufferLogic::Destroy(context, unit->indexBuffer);

			ShaderLogic::DestroyUnitDescriptor(context, unit);
		}
		unitEOs.clear();
	}

	void UnitLogic::SetPipelineName(Context *context,
									std::shared_ptr<Unit> unit,
									const std::string &name)
	{
		unit->pipelineName = name;
	}

	void UnitLogic::SetVertices(Context *context,
								std::shared_ptr<Unit> unit,
								std::vector<Vertex> &vertices)
	{
		auto vertexSize = static_cast<VkDeviceSize>(sizeof(Vertex) * vertices.size());

		unit->vertices = vertices;
		unit->vertexBuffer = BufferLogic::Create(context,
												 vertexSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
												 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferSetLogic::SetVector(context,
								  unit->vertexBuffer,
								  unit->vertices);
	}

	void UnitLogic::SetIndices(Context *context,
							   std::shared_ptr<Unit> unit,
							   std::vector<uint16_t> &indices)
	{
		auto indexSize = static_cast<VkDeviceSize>(sizeof(uint16_t) * indices.size());

		unit->indices = indices;
		unit->indexBuffer = BufferLogic::Create(context,
												indexSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
												VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferSetLogic::SetVector(context,
								  unit->indexBuffer,
								  unit->indices);
	}

	void UnitLogic::SetImage(Context *context,
							 std::shared_ptr<Unit> unit,
							 const std::string &name)
	{
		int w, h;
		unsigned char *data = Common::ResSystem::LoadImg(name, w, h);

		auto imageW = static_cast<uint32_t>(w);
		auto imageH = static_cast<uint32_t>(h);
		auto imageSize = static_cast<VkDeviceSize>(imageW * imageH * 4);

		auto tempBuffer = BufferLogic::CreateTemp(context,
												  imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
												  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferSetLogic::SetPtr(context,
							   tempBuffer,
							   data, static_cast<size_t>(imageSize));

		ImageInfo image2dInfo = {
			VK_FORMAT_R8G8B8A8_UNORM, {imageW, imageH, 1}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			1,
			VK_IMAGE_VIEW_TYPE_2D,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL};
		auto image2d = ImageLogic::CreateByInfo(context, image2dInfo);

		ImageLogic::CopyFromBuffer(context,
								   image2d,
								   tempBuffer);

		ImageLogic::TransitionLayout(context,
									 image2d,
									 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		Common::ResSystem::FreeImg(data);

		unit->image0 = image2d;

		ShaderLogic::CreateUnitDescriptor(context,
										  unit);
	}

	void UnitLogic::SetImageCube(Context *context,
								 std::shared_ptr<Unit> unit,
								 std::array<std::string, 6> names)
	{
		uint32_t imageCubeW, imageCubeH = 0;

		std::vector<unsigned char *> datas(6);

		for (auto i = 0; i < 6; i++)
		{
			int w, h;
			unsigned char *data = Common::ResSystem::LoadImg(names[i], w, h);

			imageCubeW = static_cast<uint32_t>(w);
			imageCubeH = static_cast<uint32_t>(h);

			datas[i] = data;
		}

		auto imageCubeSizeOne = static_cast<VkDeviceSize>(imageCubeW * imageCubeH * 4);
		auto imageCubeSizeTotal = imageCubeSizeOne * 6;

		auto tempBuffer = BufferLogic::CreateTemp(context,
												  imageCubeSizeTotal, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
												  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		BufferSetLogic::SetPtrVector(context,
									 tempBuffer,
									 datas, imageCubeSizeOne);

		ImageInfo imageCubeInfo = {
			VK_FORMAT_R8G8B8A8_UNORM, {imageCubeW, imageCubeH, 1}, VK_IMAGE_ASPECT_COLOR_BIT,
			// image
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
			6,
			VK_IMAGE_VIEW_TYPE_CUBE,
			// memory
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			// layout
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL};
		auto imageCube = ImageLogic::CreateByInfo(context, imageCubeInfo);

		ImageLogic::CopyFromBuffer(context,
								   imageCube,
								   tempBuffer);

		ImageLogic::TransitionLayout(context,
									 imageCube,
									 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		for (auto i = 0; i < 6; i++)
		{
			Common::ResSystem::FreeImg(datas[i]);
		}

		unit->image0 = imageCube;

		ShaderLogic::CreateUnitDescriptor(context,
										  unit);
	}

	void UnitLogic::SetObj(Context *context,
						   std::shared_ptr<Unit> unit,
						   const std::string &name, glm::vec3 &defaultColor)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;

		Common::ResSystem::LoadObjShapes(name, attrib, shapes);

		std::vector<Render::Vertex> vertices;
		std::vector<uint16_t> indices;

		std::unordered_map<Render::Vertex, uint32_t> uniqueVertices = {};

		for (const auto &shape : shapes)
		{
			for (const auto &index : shape.mesh.indices)
			{
				auto vi = index.vertex_index;
				auto ni = index.normal_index;

				Render::Vertex vertex = {};
				vertex.positionOS = {-attrib.vertices[3 * vi + 0],
									 attrib.vertices[3 * vi + 1],
									 attrib.vertices[3 * vi + 2]};
				vertex.normalOS = {attrib.normals[3 * ni + 0],
								   attrib.normals[3 * ni + 1],
								   attrib.normals[3 * ni + 2]};
				vertex.color = defaultColor;
				if (attrib.texcoords.size() > 0)
				{
					auto ui = index.texcoord_index;
					vertex.uv0 = {attrib.texcoords[2 * ui + 0], 1.0f - attrib.texcoords[2 * ui + 1]};
				}
				else
				{
					vertex.uv0 = {0.0f, 0.0f};
				}

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}

		Render::UnitLogic::SetVertices(context, unit, vertices);
		Render::UnitLogic::SetIndices(context, unit, indices);
	}
}