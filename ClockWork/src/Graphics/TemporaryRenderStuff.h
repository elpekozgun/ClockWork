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
#include "Mesh.h"

namespace CW
{
	class CW_API TempRender
	{
	public:
		std::vector<GLuint> indices;

		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> lightShader;
		std::unique_ptr<VAO> vao;
		std::unique_ptr<VBO> vbo;
		std::unique_ptr<EBO> ebo;
		std::unique_ptr<VAO> lightVao;
		std::unique_ptr<VBO> lightVbo;
		std::unique_ptr<EBO> lightEbo;
		//std::vector<unique_ptr<Texture>> Textures;
		std::vector<Texture> textures;
		std::unique_ptr<Mesh> mesh;

		/*std::unique_ptr<Texture> diffuseTexture;
		std::unique_ptr<Texture> specularTexture;*/

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

			std::vector<Vertex> vertices
			{
				Vertex{glm::vec3(-0.5f, 0.0f,  0.5f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec3(0.83f, 0.70f, 0.44f), 	 glm::vec2(0.0f, 0.0f)},      // Bottom side
				Vertex{glm::vec3(-0.5f, 0.0f, -0.5f),  glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(0.0f, 5.0f)},      // Bottom side
				Vertex{glm::vec3(0.5f, 0.0f, -0.5f),   glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(5.0f, 5.0f)},      // Bottom side
				Vertex{glm::vec3(0.5f, 0.0f,  0.5f),   glm::vec3(0.0f, -1.0f, 0.0f),   glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(5.0f, 0.0f)},      // Bottom side
				Vertex{glm::vec3(-0.5f, 0.0f,  0.5f),  glm::vec3(-0.8f, 0.5f,  0.0f),  glm::vec3(0.83f, 0.70f, 0.44f),   glm::vec2(0.0f, 0.0f)},      // Left Side
				Vertex{glm::vec3(-0.5f, 0.0f, -0.5f),  glm::vec3(-0.8f, 0.5f,  0.0f),  glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(5.0f, 0.0f)},      // Left Side
				Vertex{glm::vec3( 0.0f, 0.8f,  0.0f),  glm::vec3(-0.8f, 0.5f,  0.0f),  glm::vec3(0.92f, 0.86f, 0.76f),	 glm::vec2(2.5f, 5.0f)},      // Left Side
				Vertex{glm::vec3(-0.5f, 0.0f, -0.5f),  glm::vec3( 0.0f, 0.5f, -0.8f),  glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(5.0f, 0.0f)},      // Non-facing side
				Vertex{glm::vec3( 0.5f, 0.0f, -0.5f),  glm::vec3( 0.0f, 0.5f, -0.8f),  glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(0.0f, 0.0f)},      // Non-facing side
				Vertex{glm::vec3( 0.0f, 0.8f,  0.0f),  glm::vec3( 0.0f, 0.5f, -0.8f),  glm::vec3(0.92f, 0.86f, 0.76f),	 glm::vec2(2.5f, 5.0f)},      // Non-facing side
				Vertex{glm::vec3( 0.5f, 0.0f, -0.5f),  glm::vec3( 0.8f, 0.5f,  0.0f),  glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(0.0f, 0.0f)},      // Right side
				Vertex{glm::vec3( 0.5f, 0.0f,  0.5f),  glm::vec3( 0.8f, 0.5f,  0.0f),  glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(5.0f, 0.0f)},      // Right side
				Vertex{glm::vec3( 0.0f, 0.8f,  0.0f),  glm::vec3( 0.8f, 0.5f,  0.0f),  glm::vec3(0.92f, 0.86f, 0.76f),	 glm::vec2(2.5f, 5.0f)},      // Right side
				Vertex{glm::vec3( 0.5f, 0.0f,  0.5f),  glm::vec3( 0.0f, 0.5f,  0.8f),  glm::vec3(0.83f, 0.70f, 0.44f),	 glm::vec2(5.0f, 0.0f)},      // Facing side
				Vertex{glm::vec3(-0.5f, 0.0f,  0.5f),  glm::vec3(0.0f, 0.5f,  0.8f),   glm::vec3(0.83f, 0.70f, 0.44f), 	 glm::vec2(0.0f, 0.0f)},      // Facing side
				Vertex{glm::vec3(0.0f, 0.8f,  0.0f),   glm::vec3(0.0f, 0.5f,  0.8f),   glm::vec3(0.92f, 0.86f, 0.76f),	 glm::vec2(2.5f, 5.0f)}      // Facing side
			};

			indices.push_back(0);  
			indices.push_back(1);  
			indices.push_back(2);  
			indices.push_back(0);  
			indices.push_back(2);  
			indices.push_back(3);  
			indices.push_back(4); 
			indices.push_back(6); 
			indices.push_back(5); 
			indices.push_back(7); 
			indices.push_back(9); 
			indices.push_back(8); 
			indices.push_back(10);
			indices.push_back(12);
			indices.push_back(11);
			indices.push_back(13);
			indices.push_back(15);
			indices.push_back(14);

			//Textures.push_back(std::make_unique<Texture>(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\crate.png)", "Diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_REPEAT));
			//Textures.push_back(std::make_unique<Texture>(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\cratespecular.png)", "Specular", 1, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_REPEAT));


			textures.push_back(Texture(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\crate.png)", "Diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_REPEAT));
			textures.push_back(Texture(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\cratespecular.png)", "Specular", 1, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_REPEAT));

			mesh = std::make_unique<Mesh>(vertices, indices, textures);

			
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

			auto vertexShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\DefaultVertex.glsl)", ShaderType::Vertex);
			auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\DefaultFragment.glsl)", ShaderType::Fragment);

			shader = Shader::CreateShader("shader", { vertexShader, fragmentShader });

			vao = std::make_unique<VAO>();
			vao->Bind();

			vbo = std::make_unique<VBO>(vertices);
			ebo = std::make_unique<EBO>(indices);

			vao->LinkAttribArray<float>(*vbo, 0, 3, GL_FLOAT, 11, 0);
			vao->LinkAttribArray<float>(*vbo, 1, 3, GL_FLOAT, 11, 3);
			vao->LinkAttribArray<float>(*vbo, 2, 3, GL_FLOAT, 11, 6);
			vao->LinkAttribArray<float>(*vbo, 3, 2, GL_FLOAT, 11, 9);

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
			
			shader->SetFloat("Shineness", 32.0f);
			shader->setVec3("pointLights[0].position", lightPos);
			shader->setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
			shader->setVec3("pointLights[0].ambient", 0.15f, 0.15f, 0.15f);
			shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			shader->SetFloat("pointLights[0].Kconstant", 1.0f);
			shader->SetFloat("pointLights[0].Klinear", 0.09f);
			shader->SetFloat("pointLights[0].Kquad", 0.032f);
			shader->SetBool("IsBlinnPhong", true);


			// Order of samplers is important here.
			shader->SetTexture("Diffuse0", 0); 
			shader->SetTexture("Specular0", 1);


			lightShader->Use();
			lightShader->setVec4("LightColor", lightColor);
			lightShader->setMat4("Model", lightModel);

		}

		void Render(float dt)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto camMat = camera->CameraMatrix();

			shader->Use();
			shader->setMat4("CamMat", camMat);
			shader->setVec3("eyePosition", camera->Position);

			mesh->Draw(*shader, camMat, camera->Position);

			//vao->Bind();
			//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

			glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

			lightShader->Use();
			lightShader->setMat4 ("CamMat", camMat);

			lightVao->Bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		~TempRender()
		{
			mesh.reset();
			//delete[] indices;
		}
	};
}