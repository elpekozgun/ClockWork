#pragma once

#include "Core/Core.h"
#include "Mesh.h"

#include <vector>
#include <string>
#include <istream>

namespace CW
{
	class CW_API Model
	{
	public:
		Model(const char* file);
		
		void Draw(Shader& shader, glm::mat4 camMat, glm::vec3 camPos);

	private:
		void LoadModel(std::string path);

		std::vector<Mesh> Meshes;
		std::vector<Texture> Textures;

		const char* _file;
		std::vector<unsigned char> data;
	};
}