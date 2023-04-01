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
		std::unique_ptr<VAO> vao;
		std::unique_ptr<VBO> vbo;
		std::unique_ptr<EBO> ebo;
		std::unique_ptr<VAO> lightVao;
		std::unique_ptr<VBO> lightVbo;
		std::unique_ptr<EBO> lightEbo;
		//std::unique_ptr<Mesh> mesh;
		//std::unique_ptr<Mesh> lightMesh;

		CW::CameraComponent* camera;


		std::unique_ptr<Model> CharacterModel;

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

			textures.push_back(Texture(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\crate.png)", "Diffuse", 0));
			textures.push_back(Texture(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\cratespecular.png)", "Specular", 1));

			//mesh = std::make_unique<Mesh>(vertices, indices, textures);

			auto vertexShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\DefaultVertex.glsl)", ShaderType::Vertex);
			auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\DefaultFragment.glsl)", ShaderType::Fragment);

			modelShader = Shader::CreateShader("shader", { vertexShader, fragmentShader });


			//CharacterModel = std::make_unique<Model>(R"(C:\_Dev\ClockWork\ClockWork\res\3DModel\maria\Maria WProp J J Ong.dae)");
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
			lightShader = Shader::CreateShader("shader", { lightVertex, lightFragment });

			//lightMesh = std::make_unique<Mesh>(LightVertices, LightIndices, std::vector<Texture>());

			glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
			glm::mat4 lightModel = glm::mat4(1);
			lightModel = glm::translate(lightModel, lightPos);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.01f));

			modelShader->Use();
			modelShader->setVec4("LightColor", lightColor);
			modelShader->setMat4("Model", model);
			
			modelShader->SetFloat("Shineness", 32.0f);
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

			modelShader->setMat4("Model", model);
			modelShader->setMat4("CamMat", camMat);
			modelShader->setVec3("eyePosition", camera->Position);

			CharacterModel->Draw(*modelShader, camMat, camera->Position);

			/*modelShader->Use();
			modelShader->setMat4("CamMat", camMat);
			modelShader->setVec3("eyePosition", camera->Position);*/
			//mesh->Draw(*modelShader, camMat, camera->Position);


			//glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
			//lightShader->Use();
			//lightShader->setMat4 ("CamMat", camMat);
			//lightMesh->Draw(*lightShader, camMat, camera->Position);

		}

		~TempRender()
		{
			//mesh.reset();
			//lightMesh.reset();
		}
	};
}