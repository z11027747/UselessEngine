
#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <image/stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "common/res_system.h"
#include "common/log_system.h"

namespace Common
{
	std::vector<char> ResSystem::ReadAllBytes(const std::string &fileName)
	{
		std::ifstream file(fileName, std::ios::ate | std::ios::binary);

		auto fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	std::vector<std::string> ResSystem::ReadAllLines(const std::string &fileName)
	{
		std::ifstream file(fileName);

		std::vector<std::string> lines;
		std::string line;
		while (std::getline(file, line))
		{
			if (!line.empty())
				lines.push_back(line);
		}

		file.close();
		return lines;
	}

	void ResSystem::WriteFile(const std::string &fileName, const std::string &context)
	{
		std::ofstream outputFile(fileName);
		outputFile << context;
		outputFile.close();
	}

	std::unordered_map<std::string, ResImage> ResSystem::imgMap = {};
	std::unordered_map<std::string, ResObj> ResSystem::objMap = {};

	ResImage &ResSystem::LoadImg(const std::string &fileName)
	{
		auto it = imgMap.find(fileName);
		if (it == imgMap.end())
		{
			int w, h, comp;
			auto data = stbi_load(fileName.data(), &w, &h, &comp, STBI_rgb_alpha);
			if (!data)
			{
				LogSystem::Exception("failed to load texture image!");
			}

			ResImage img = {};
			img.width = w;
			img.height = h;
			img.data = data;
			imgMap.emplace(fileName, img);
		}

		return imgMap[fileName];
	}

	ResObj &ResSystem::LoadObj(const std::string &fileName)
	{
		auto it = imgMap.find(fileName);
		if (it == imgMap.end())
		{
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string err;

			if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.data()))
			{
				LogSystem::Exception(err);
			}

			ResObj obj = {};
			obj.attrib = attrib;
			obj.shapes = shapes;
			objMap.emplace(fileName, obj);
		}

		return objMap[fileName];
	}
}