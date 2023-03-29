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

namespace CW
{
	class CW_API TempRender
	{
	public:
		//Shader& shaderProgram;
		GLuint indices[9];

		std::unique_ptr<Shader> shader;
		std::unique_ptr<VAO> vao;
		std::unique_ptr<VBO> vbo;
		std::unique_ptr<EBO> ebo;

		TempRender()
		{
			indices[0] = 0;
			indices[1] = 3;
			indices[2] = 5;
			indices[3] = 3;
			indices[4] = 2;
			indices[5] = 4;
			indices[6] = 5;
			indices[7] = 4;
			indices[8] = 1;

			GLfloat vertices[] =
			{
				-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
				0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
				0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
				-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
				0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
				0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f
			};


			auto vertexShader = Shader::CreateShaderSource(R"(res/Shader/SimpleVertex.glsl)", ShaderType::Vertex);
			auto fragmentShader = Shader::CreateShaderSource(R"(res/Shader/SimpleFragment.glsl)", ShaderType::Fragment);

			shader = Shader::CreateShader("shader", { vertexShader, fragmentShader });

			
			vao = std::make_unique<VAO>();
			vao->Bind();

			vbo = std::make_unique<VBO>(vertices, sizeof(vertices));
			ebo = std::make_unique<EBO>(indices, sizeof(indices));

			vao->LinkVBO<float>(*vbo, 0, 3);

			vao->Unbind();
			vbo->Unbind();
			ebo->Unbind();
		}

		void Render(float dt)
		{
			auto tStart = CW::Clock::now();

			glClearColor(0.2f * dt, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			shader->Use();
			glBindVertexArray(vao->Id);
			glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		}
	};
}