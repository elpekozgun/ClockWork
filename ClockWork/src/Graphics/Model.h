#pragma once

#include "Core/Core.h"
#include "Core/Defines.h"
#include "glm.hpp"
#include "Json.h"
#include <vector>
#include <string>
#include <istream>

using Json = nlohmann::json;

namespace CW
{
	class CW_API Model
	{
	public:
		Model(const char* file);
		void Draw();

	private:
		const char* _file;
		std::vector<unsigned char> data;
		Json _json;
	};
}