#pragma once
#include "glad/glad.h"
#include "glm.hpp"
#include <string>
#include "Core/Defines.h"
#include "Shader.h"
#include <memory>
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "stb_image.h"

namespace CW
{
	GLuint uniformId;
	class CW_API TempRender
	{
	public:
		GLuint indices[9];

		std::unique_ptr<Shader> shader;
		std::unique_ptr<VAO> vao;
		std::unique_ptr<VBO> vbo;
		std::unique_ptr<EBO> ebo;

		TempRender()
		{
			indices[0] = 0;
			indices[1] = 2;
			indices[2] = 1;
			indices[3] = 0;
			indices[4] = 3;
			indices[5] = 2;


			GLfloat vertices[] =
			{
				-0.5f, -0.5f, 0.0f , 1.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, 0.0f , 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.0f , 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.0f , 1.0f, 1.0f, 0.0f,
			};
			

			auto vertexShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\SimpleVertex.glsl)", ShaderType::Vertex);
			auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\SimpleFragment.glsl)", ShaderType::Fragment);

			shader = Shader::CreateShader("shader", { vertexShader, fragmentShader });

			vao = std::make_unique<VAO>();
			vao->Bind();

			vbo = std::make_unique<VBO>(vertices, sizeof(vertices));
			ebo = std::make_unique<EBO>(indices, sizeof(indices));

			vao->LinkAttribArray<float>(* vbo, 0, 3, GL_FLOAT, 6, 0);
			vao->LinkAttribArray<float>(*vbo, 1, 3, GL_FLOAT, 6 , 3);

			vao->Unbind();
			vbo->Unbind();
			ebo->Unbind();

		}

		void Render(float dt)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			shader->Use();
			shader->SetFloat("scale", 0);
			glBindVertexArray(vao->Id);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}
	};
}