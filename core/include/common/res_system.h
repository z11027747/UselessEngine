#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace Common {

	class ResSystem final {
	public:

		static std::vector<char> ReadFile(const std::string&);
		static void WriteFile(const std::string&, const std::string&);

		static unsigned char* LoadImg(const std::string&, int&, int&);
		static void FreeImg(unsigned char*);
	};

}
