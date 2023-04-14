#pragma once

#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "glad/glad.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "glm.hpp"
#include "Graphics/ShaderFactory.h"

#include <vector>
#include <execution>

namespace CW
{
	class CW_API AABBRenderer : public ISystem
	{
	public:
		AABBRenderer()
		{
			auto vs = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\aabb.vert)", ShaderType::Vertex);
			auto fs = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\aabb.frag)", ShaderType::Fragment);

			_aabbShader = ShaderFactory::CreateShader("aabb", { vs, fs });

			std::vector<glm::vec3> vertices = 
			{
				glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-back-left
				glm::vec3(-0.5f,  0.5f, -0.5f), // top-back-left
				glm::vec3(0.5f,  0.5f, -0.5f), // top-back-right
				glm::vec3(0.5f, -0.5f, -0.5f), // bottom-back-right
				glm::vec3(-0.5f, -0.5f,  0.5f), // bottom-front-left
				glm::vec3(-0.5f,  0.5f,  0.5f), // top-front-left
				glm::vec3(0.5f,  0.5f,  0.5f), // top-front-right
				glm::vec3(0.5f, -0.5f,  0.5f)  // bottom-front-right
			};

			// Create vertex indices for the edges of the box
			indices = 
			{
				0, 1, 1, 2, 2, 3, 3, 0, // back face
				4, 5, 5, 6, 6, 7, 7, 4, // front face
				0, 4, 1, 5, 2, 6, 3, 7  // side edges
			};

			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

			GLuint ebo;
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			_aabbVao.Bind();

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBindVertexArray(0);

		}

		virtual void Update(float deltaTime) override;

	private:
		void Render(AABBComponent& aabb, TransformComponent& transform, CameraComponent& camera);

		Shader _aabbShader;
		VAO _aabbVao;
		std::vector<unsigned int> indices;
		bool _showBox = false;
	};
}
