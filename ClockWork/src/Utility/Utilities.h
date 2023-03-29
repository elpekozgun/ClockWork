#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace CW
{
	class Utilities
	{
	public:

		std::string static ReadFile(const char* filePath)
		{
			std::ifstream file(filePath);
			if (!file.is_open())
			{
				std::cerr << "failed to open file: " << filePath << "\n";
				return "";
			}

			std::stringstream ss;

			ss << file.rdbuf();
			file.close();

			return ss.str();
		}
	};
}

