#pragma once
#include <string>
#include <memory>
#include <iostream>

#include "glad/glad.h"
#include "glm.hpp"
#include "Core/Defines.h"
#include "gtc/matrix_transform.hpp"
#include "System/CameraSystem.h"

#include "Model.h"

namespace CW
{
	class CW_API TempRender
	{
	public:
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		std::unique_ptr<Shader> modelShader;
		std::unique_ptr<Shader> lightShader;
		std::unique_ptr<VAO> lightVao;
		std::unique_ptr<VBO> lightVbo;
		std::unique_ptr<EBO> lightEbo;
		
		std::unique_ptr<Mesh> lightMesh;
		std::unique_ptr<Mesh> groundMesh;

		CW::CameraComponent* camera;


		std::unique_ptr<Model> CharacterModel;

		TempRender()
		{
			CameraComponent camera;
			camera.Position = glm::vec3(-1.45f, 0.9f, -0.8f);
			camera.Forward = glm::vec3(0.832167f, -0.377841f, 0.405875f);
			camera.Up = glm::vec3(0.339601f, 0.925871f, 0.165634f);
			camera.Width = 640;
			camera.height = 480;
			camera.FoV = 60;
			camera.speed = 0.01f;
			camera.sensitivity = 0.1f;

			auto vertexShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\DefaultVertex.glsl)", ShaderType::Vertex);
			auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\DefaultFragment.glsl)", ShaderType::Fragment);

			modelShader = std::make_unique<Shader>(Shader::CreateShader("shader", { vertexShader, fragmentShader }));

			std::vector<Vertex> vertices =
			{
				Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec2(0.0f, 0.0f)},
				Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec2(0.0f, 1.0f)},
				Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),glm::vec2(1.0f, 1.0f)},
				Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),glm::vec2(1.0f, 0.0f)}
			};
						// Indices for vertices order
			std::vector<GLuint> indices =
			{
				0, 1, 2,
				0, 2, 3
			};

			textures.push_back(Texture(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\crate.png)", "Diffuse", 0));
			textures.push_back(Texture(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\cratespecular.png)", "Specular", 1));

			groundMesh = std::make_unique<Mesh>(vertices, indices, textures);

			CharacterModel = std::make_unique<Model>(R"(C:/_Dev/ClockWork/ClockWork/res/3DModel/maria/Maria WProp J J Ong.dae)");


			//-------------------------------------------

			std::vector<Vertex> LightVertices
			{
				Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
				Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
				Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
				Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
				Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
				Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
				Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
				Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
			};

			std::vector<unsigned int> LightIndices
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


			auto lightVertex = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\LightVertex.glsl)", ShaderType::Vertex);
			auto lightFragment = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\LightFragment.glsl)", ShaderType::Fragment);
			lightShader = std::make_unique<Shader>(Shader::CreateShader("shader", { lightVertex, lightFragment }));

			lightMesh = std::make_unique<Mesh>(LightVertices, LightIndices, std::vector<Texture>());

			glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec3 lightPos = glm::vec3(0.0f, 1.9f, 0.1f);
			glm::mat4 lightModel = glm::mat4(1);
			lightModel = glm::translate(lightModel, lightPos);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.01f));

			modelShader->Use();
			modelShader->setVec4("LightColor", lightColor);
			modelShader->setMat4("Model", model);
			
			modelShader->SetFloat("Shineness", 32.0f);


			modelShader->setVec3("spotlight.ambient", 0.2f, 0.2f, 0.2f);
			modelShader->setVec3("spotlight.diffuse", 0.8f, 0.8f, 0.8f);
			modelShader->setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
			modelShader->SetFloat("spotlight.cutOff", cos(glm::radians(20.5f)));
			modelShader->SetFloat("spotlight.outerCutOff", cos(glm::radians(26.0f)));
			modelShader->SetFloat("spotlight.Kconstant", 1.0f);
			modelShader->SetFloat("spotlight.Klinear", 0.027f);
			modelShader->SetFloat("spotlight.Kquad", 0.0028f);

			modelShader->setVec3("pointLights[0].position", lightPos);
			modelShader->setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
			modelShader->setVec3("pointLights[0].ambient", 0.15f, 0.15f, 0.15f);
			modelShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			modelShader->SetFloat("pointLights[0].Kconstant", 1.0f);
			modelShader->SetFloat("pointLights[0].Klinear", 0.09f);
			modelShader->SetFloat("pointLights[0].Kquad", 0.032f);
			modelShader->SetBool("IsBlinnPhong", true);

			lightShader->Use();
			lightShader->setVec4("LightColor", lightColor);
			lightShader->setMat4("Model", lightModel);
		}

		void Render(float dt)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto camMat = camera->CameraMatrix();

			modelShader->Use();

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(glm::vec3(0.01f));

			modelShader->setVec3("spotlight.position", camera->Position);
			modelShader->setVec3("spotlight.direction", camera->Forward);

			modelShader->setMat4("Model", model);
			modelShader->setMat4("CamMat", camMat);
			modelShader->setVec3("eyePosition", camera->Position);

			CharacterModel->Draw(*modelShader, camMat, camera->Position);

			model = glm::mat4(1.0f);
			modelShader->setMat4("Model", model);
			groundMesh->Draw(*modelShader, camMat, camera->Position);

			lightShader->Use();
			lightShader->setMat4 ("CamMat", camMat);
			lightMesh->Draw(*lightShader, camMat, camera->Position);

		}

		~TempRender()
		{
			//mesh.reset();
			//lightMesh.reset();
		}
	};
}