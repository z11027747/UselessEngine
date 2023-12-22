#pragma once

#include <iostream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

namespace Common {

	class LogSystem {
	public:

		static void Debug(const std::string& message) {
			auto now = std::time(nullptr);
			std::tm local_time = {};
			localtime_s(&local_time, &now);
			std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "]";
			std::cout << "[Debug]";
			std::cout << " " << message;
			std::cout << std::endl;
		}

		static void Info(const std::string& message) {
			auto now = std::time(nullptr);
			std::tm local_time = {};
			localtime_s(&local_time, &now);
			std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "]";
			std::cout << "[Info]";
			std::cout << " " << "\033[1;32m" << message << "\033[0m";
			std::cout << "\n";
			std::cout << std::endl;
		}

		static void Error(const std::string& message) {
			auto now = std::time(nullptr);
			std::tm local_time = {};
			localtime_s(&local_time, &now);
			std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "]";
			std::cout << "[Error]";
			std::cerr << " " << "\033[1;31m" << message << "\033[0m";
			std::cout << "\n";
			std::cout << std::endl;
		}

		static void Exception(const std::string& message) {
			Error(message);
			throw std::runtime_error(message);
		}

	};

}