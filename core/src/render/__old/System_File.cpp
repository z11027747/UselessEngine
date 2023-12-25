
#include "tool/system.h"
#include "common/res_system.h"

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
	return Common::ResSystem::LoadImg(fileName, w, h);
}

void ToolSystem::FreeImg(unsigned char* data) {
	Common::ResSystem::FreeImg(data);
}
