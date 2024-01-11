#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>

namespace Common
{
	class LogSystem final
	{
	public:
		template <typename... Args>
		static std::string ConcatenateArgs(Args &&...args)
		{
			std::ostringstream oss;
			(oss << ... << std::forward<Args>(args));
			return oss.str();
		}

		template <typename... Args>
		static void Debug(Args &&...args)
		{
			auto message = ConcatenateArgs(std::forward<Args>(args)...);

			auto now = std::time(nullptr);
			std::tm local_time = {};
			localtime_s(&local_time, &now);
			std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "]";
			std::cout << "[Debug]";
			std::cout << message;
			std::cout << "\n";
			std::cout << std::endl;
		}

		template <typename... Args>
		static void Info(Args &&...args)
		{
			auto message = ConcatenateArgs(std::forward<Args>(args)...);

			auto now = std::time(nullptr);
			std::tm local_time = {};
			localtime_s(&local_time, &now);
			std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "]";
			std::cout << "[Info]";
			std::cout << "\033[1;32m" << message << "\033[0m";
			std::cout << "\n";
			std::cout << std::endl;
		}

		template <typename... Args>
		static void Error(Args &&...args)
		{
			auto message = ConcatenateArgs(std::forward<Args>(args)...);

			auto now = std::time(nullptr);
			std::tm local_time = {};
			localtime_s(&local_time, &now);
			std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "]";
			std::cout << "[Error]";
			std::cerr << "\033[1;31m" << message << "\033[0m";
			std::cout << "\n";
			std::cout << std::endl;
		}

		template <typename... Args>
		static void Exception(Args &&...args)
		{
			auto message = ConcatenateArgs(std::forward<Args>(args)...);

			auto now = std::time(nullptr);
			std::tm local_time = {};
			localtime_s(&local_time, &now);
			std::cout << "[" << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S") << "]";
			std::cout << "[Exception]";
			std::cerr << "\033[1;31m" << message << "\033[0m";
			std::cout << "\n";
			std::cout << std::endl;

			throw std::runtime_error(message);
		}
	};

}