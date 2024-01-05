#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <memory>
#include <string>
#include "render/unit/unit_comp.h"

class Context;

namespace Render
{
	class UnitLogic final
	{
	public:
		static void Destroy(Context *);

		static void SetPipelineName(Context *,
									std::shared_ptr<Unit>, const std::string &);

		static void SetVertices(Context *,
								std::shared_ptr<Unit>, std::vector<Vertex> &);

		static void SetIndices(Context *,
							   std::shared_ptr<Unit>, std::vector<uint16_t> &);

		static void SetImage(Context *,
							 std::shared_ptr<Unit>, const std::string &);

		static void SetImageCube(Context *,
								 std::shared_ptr<Unit>, std::array<std::string, 6>);

		static void SetObj(Context *,
						   std::shared_ptr<Unit>, const std::string &,
						   glm::vec3 &defaultColor = glm::vec3(1.0f, 1.0f, 1.0f));
	};

}