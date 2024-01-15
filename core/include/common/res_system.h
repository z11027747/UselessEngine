#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <obj/tiny_obj_loader.h>

namespace Common
{
	struct ResImage
	{
		int width;
		int height;
		unsigned char *data;
	};

	struct ResObj
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
	};

	class ResSystem final
	{
	public:
		static std::vector<char> ReadAllBytes(const std::string &);
		static std::vector<std::string> ReadAllLines(const std::string &);
		static void WriteFile(const std::string &, const std::string &);

		static std::unordered_map<std::string, ResImage> imgMap;
		static std::unordered_map<std::string, ResObj> objMap;

		static ResImage &LoadImg(const std::string &);
		static ResObj &LoadObj(const std::string &);
	};
}
