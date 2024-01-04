
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "common/res_system.h"

namespace Common
{

	std::vector<char> ResSystem::ReadFile(const std::string &fileName)
	{
		std::ifstream file(fileName, std::ios::ate | std::ios::binary);

		auto fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	void ResSystem::WriteFile(const std::string &fileName, const std::string &context)
	{
		std::ofstream outputFile(fileName);
		outputFile << context << std::endl;
		outputFile.close();
	}

	unsigned char *ResSystem::LoadImg(const std::string &fileName, int &w, int &h)
	{
		int comp;
		auto data = stbi_load(fileName.data(), &w, &h, &comp, STBI_rgb_alpha);
		if (!data)
		{
			throw std::runtime_error("failed to load texture image!");
		}

		return data;
	}

	void ResSystem::FreeImg(unsigned char *data)
	{
		stbi_image_free(data);
	}

	bool ResSystem::LoadObjShapes(const std::string &fileName,
								  tinyobj::attrib_t &attrib, std::vector<tinyobj::shape_t> &shapes)
	{
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.data()))
		{
			throw std::runtime_error(err);
		}

		return true;
	}
}