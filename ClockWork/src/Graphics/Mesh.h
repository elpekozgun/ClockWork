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
	struct CW_API MeshComponent
	{
		std::vector<Vertex> Vertices;
		std::vector<Texture> Textures;
		std::vector<unsigned int> Indices;
		VAO Vao;
		Shader Shader;

		void Setup()
		{
			Vao.Bind();

			EBO ebo(Indices);
			VBO vbo(Vertices);

			unsigned int stride = sizeof(Vertex) / sizeof(float);

			Vao.LinkAttribArray<float>(vbo, 0, 3, GL_FLOAT, stride, 0);
			Vao.LinkAttribArray<float>(vbo, 1, 3, GL_FLOAT, stride, 3);
			Vao.LinkAttribArray<float>(vbo, 2, 3, GL_FLOAT, stride, 6);
			Vao.LinkAttribArray<float>(vbo, 3, 2, GL_FLOAT, stride, 9);

			Vao.Unbind();
			vbo.Unbind();
			ebo.Unbind();
		}
	};

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
