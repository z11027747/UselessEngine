
#define STB_IMAGE_IMPLEMENTATION
#include <image/stb_image.h>
#include "tool/system.h"

std::vector<char> ToolSystem::ReadFile(const std::string& fileName) {

	std::ifstream file(fileName, std::ios::ate | std::ios::binary);

	auto fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

void ToolSystem::WriteFile(const std::string& fileName, const std::string& context) {
	std::ofstream outputFile(fileName);
	outputFile << context << std::endl;
	outputFile.close();
}

unsigned char* ToolSystem::LoadImg(const std::string& fileName, int& w, int& h) {

	//STBI_rgb_alpha通道参数可以强制载入alpha通道
	//	即使图像数据不包含这一通道，也会被添加上一个默认的alpha值作为alpha通道的图像数据

	int channels;
	auto data = stbi_load(fileName.data(), &w, &h, &channels, STBI_rgb_alpha);
	if (!data) {
		throw std::runtime_error("failed to load texture image!");
	}

	return data;
}

void ToolSystem::FreeImg(unsigned char* data) {
	stbi_image_free(data);
}
