#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class ToolSystem final {
public:

	static std::vector<char> ReadFile(const std::string&);
	static void WriteFile(const std::string&, const std::string&);

};