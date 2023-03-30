#pragma once
#include <string>
#include <memory>
#include <iostream>

#include "glad/glad.h"
#include "glm.hpp"
#include "Core/Defines.h"
#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "System/CameraSystem.h"


namespace CW
{
	class CW_API TempRender
	{
	public:
		GLuint indices[18];

		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> lightShader;
		std::unique_ptr<VAO> vao;
		std::unique_ptr<VBO> vbo;
		std::unique_ptr<EBO> ebo;
		std::unique_ptr<VAO> lightVao;
		std::unique_ptr<VBO> lightVbo;
		std::unique_ptr<EBO> lightEbo;
		std::unique_ptr<Texture> diffuseTexture;

		// for now we keep a reference to camera component here.
		CW::CameraComponent* camera;

		TempRender()
		{
			ECS& _ecs = ECS::Instance();

			auto entity = _ecs.CreateEntity();
			camera = &(_ecs.GetComponent <CW::CameraComponent>(entity));

			CameraComponent c{
			glm::vec3(-1.45f, 0.9f, -0.8f), glm::vec3(0.832167f, - 0.377841f, 0.405875f), glm::vec3(0.339601f, 0.925871f, 0.165634f),
				640, 480, 60, 0.01f, 0.1f };

			_ecs.AddComponent<CameraComponent>(entity, c);

			GLfloat vertices[] =
			{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
				-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
				-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
				 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
				 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

				-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
				-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

				-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
				 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

				 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
				 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

				 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
				-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
			};

			indices[0] = 0; indices[6] = 4;  indices[12] = 10;
			indices[1] = 1;	indices[7] = 6;	 indices[13] = 12;
			indices[2] = 2;	indices[8] = 5;	 indices[14] = 11;
			indices[3] = 0;	indices[9] = 7;	 indices[15] = 13;
			indices[4] = 2;	indices[10] = 9; indices[16] = 15;
			indices[5] = 3;	indices[11] = 8; indices[17] = 14;
			
			GLfloat lightVertices[] =
			{ //     COORDINATES     //
				-0.1f, -0.1f,  0.1f,
				-0.1f, -0.1f, -0.1f,
				 0.1f, -0.1f, -0.1f,
				 0.1f, -0.1f,  0.1f,
				-0.1f,  0.1f,  0.1f,
				-0.1f,  0.1f, -0.1f,
				 0.1f,  0.1f, -0.1f,
				 0.1f,  0.1f,  0.1f
			};

			GLuint lightIndices[] =
			{
				0, 1, 2,
				0, 2, 3,
				0, 4, 7,
				0, 7, 3,
				3, 7, 6,
				3, 6, 2,
				2, 6, 5,
				2, 5, 1,
				1, 5, 4,
				1, 4, 0,
				4, 5, 6,
				4, 6, 7
			};

			auto vertexShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\SimpleVertex.glsl)", ShaderType::Vertex);
			auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\SimpleFragment.glsl)", ShaderType::Fragment);

			shader = Shader::CreateShader("shader", { vertexShader, fragmentShader });

			vao = std::make_unique<VAO>();
			vao->Bind();

			vbo = std::make_unique<VBO>(vertices, sizeof(vertices));
			ebo = std::make_unique<EBO>(indices, sizeof(indices));

			vao->LinkAttribArray<float>(*vbo, 0, 3, GL_FLOAT, 11, 0);
			vao->LinkAttribArray<float>(*vbo, 1, 3, GL_FLOAT, 11, 3);
			vao->LinkAttribArray<float>(*vbo, 2, 2, GL_FLOAT, 11, 6);
			vao->LinkAttribArray<float>(*vbo, 3, 3, GL_FLOAT, 11, 8);

			vao->Unbind();
			vbo->Unbind();
			ebo->Unbind();

			auto lightVertex = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\LightVertex.glsl)", ShaderType::Vertex);
			auto lightFragment = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\LightFragment.glsl)", ShaderType::Fragment);
			lightShader = Shader::CreateShader("shader", { lightVertex, lightFragment });

			lightVao = std::make_unique<VAO>();
			lightVao->Bind();

			lightVbo = std::make_unique<VBO>(lightVertices, sizeof(lightVertices));
			lightEbo = std::make_unique<EBO>(lightIndices, sizeof(lightIndices));

			lightVao->LinkAttribArray<float>(*lightVbo, 0, 3, GL_FLOAT, 3, 0);

			vao->Unbind();
			vbo->Unbind();
			ebo->Unbind();

			glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
			glm::mat4 lightModel = glm::mat4(1);
			lightModel = glm::translate(lightModel, lightPos);

			glm::mat4 model = glm::mat4(1.0f);

			shader->Use();
			shader->setVec4("LightColor", lightColor);
			shader->setMat4("Model", model);
			shader->setVec3("LightPos", lightPos);

			lightShader->Use();
			lightShader->setVec4("LightColor", lightColor);
			lightShader->setMat4("Model", lightModel);


			diffuseTexture = std::make_unique<Texture>
				(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\crate.png)", 
				 GL_TEXTURE_2D,GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_REPEAT);
			shader->SetTexture("diffuse", 0);

		}

		void Render(float dt)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto camMat = camera->CameraMatrix();
													
			shader->Use();
			shader->setMat4("CamMat", camMat);
			//shader->setMat4("Model", model);
			shader->setVec3("CamPos", camera->Position);

			diffuseTexture->Bind();

			vao->Bind();
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

			glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

			lightShader->Use();
			lightShader->setMat4 ("CamMat", camMat);

			lightVao->Bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		~TempRender()
		{
			delete[] indices;
		}
	};
}