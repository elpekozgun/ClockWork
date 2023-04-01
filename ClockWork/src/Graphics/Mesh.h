#pragma once

#include "Core/Core.h"
#include "glm.hpp"
#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "EBO.h"
#include "Vertex.h"

#include <vector>

namespace CW
{
	class CW_API Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
		~Mesh();
		void Draw(Shader& shader, glm::mat4 camMat, glm::vec3 camPos);


		std::vector<Vertex> Vertices;
		std::vector<Texture> Textures;
		std::vector<unsigned int> Indices;

		void Delete();

	private:
		void Setup();
		

		VAO _vao;
	};



}
