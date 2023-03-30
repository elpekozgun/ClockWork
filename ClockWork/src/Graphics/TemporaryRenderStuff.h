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
		std::unique_ptr<VAO> vao;
		std::unique_ptr<VBO> vbo;
		std::unique_ptr<EBO> ebo;
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
			{ //     COORDINATES     /        COLORS      /   TexCoord  //
				-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
				-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
				 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
				 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
				 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
			};

			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 0;
			indices[4] = 2;
			indices[5] = 3;
			indices[6] = 0;
			indices[7] = 1;
			indices[8] = 4;
			indices[9] = 1;
			indices[10] = 2;
			indices[11] = 4;
			indices[12] = 2;
			indices[13] = 3;
			indices[14] = 4;
			indices[15] = 3;
			indices[16] = 0;
			indices[17] = 4;


			auto vertexShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\SimpleVertex.glsl)", ShaderType::Vertex);
			auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\SimpleFragment.glsl)", ShaderType::Fragment);

			shader = Shader::CreateShader("shader", { vertexShader, fragmentShader });

			vao = std::make_unique<VAO>();
			vao->Bind();

			vbo = std::make_unique<VBO>(vertices, sizeof(vertices));
			ebo = std::make_unique<EBO>(indices, sizeof(indices));

			vao->LinkAttribArray<float>(*vbo, 0, 3, GL_FLOAT, 8, 0);
			vao->LinkAttribArray<float>(*vbo, 1, 3, GL_FLOAT, 8, 3);
			vao->LinkAttribArray<float>(*vbo, 2, 2, GL_FLOAT, 8, 6);

			vao->Unbind();
			vbo->Unbind();
			ebo->Unbind();

			diffuseTexture = std::make_unique<Texture>
				(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\crate.png)", 
				 GL_TEXTURE_2D,GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_REPEAT);
			shader->SetTexture("diffuse", 0);

		}

		void Render(float dt)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader->Use();

			//glm::mat4 model = glm::mat4(1.0f);
			//glm::mat4 view = glm::mat4(1.0f);
			//glm::mat4 projection = glm::mat4(1.0f);

			auto camMat = camera->CameraMatrix();
			//camMat = camMat * model;

			/*model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
			view = glm::translate(view, glm::vec3(0,-0.5f, -2.0f));
			projection = glm::perspective(glm::radians(60.0f), (float)(640 / 480), 0.1f, 100.0f);*/
			
			shader->setMat4("camMat", camMat);
			//shader->setMat4("Model", model);
			//shader->setMat4("View", view);
			//shader->setMat4("Projection", projection);
			shader->SetFloat("scale", 0);
			diffuseTexture->Bind();

			glBindVertexArray(vao->Id);
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		}

		~TempRender()
		{
			delete[] indices;
		}
	};
}