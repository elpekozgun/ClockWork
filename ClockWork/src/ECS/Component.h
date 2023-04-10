#pragma once

#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "glm.hpp"
#include "Graphics/AABB.h"
#include "Graphics/Shader.h"
#include "Graphics/VAO.h"
#include "Graphics/EBO.h"
#include "Graphics/Texture.h"


#include <typeinfo>

using namespace glm;

namespace CW
{
	struct CW_API SingletonComponent { };


	struct CW_API TransformComponent
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		glm::mat4 GetMatrix()
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, Position);
			glm::quat rotation = glm::quat(glm::vec3(Rotation.x, Rotation.y, Rotation.z));
			glm::mat4 quatMatrix = glm::toMat4(rotation);
			model *= quatMatrix;
			model = glm::scale(model, Scale);
			return model;
		}

		glm::vec3 Forward()
		{
			glm::quat rotation = glm::quat(glm::vec3(Rotation.x, Rotation.y, Rotation.z));
			glm::mat4 quatMatrix = glm::toMat4(rotation);
			return quatMatrix * glm::vec4(0, 0, 1, 0);
		}

	};

	struct CW_API GravityComponent 
	{
		vec3 _force;
	};

	struct CW_API RigidBodyComponent 
	{
		vec3 velocity;
		vec3 acceleration;
	};
	
	struct CW_API Player
	{
		float moveSpeed;
		float turnspeed;
	};


	struct CW_API PhysicsComponent
	{
		vec3 Acceleration;
		vec3 Velocity;
	};

	struct CW_API RenderableComponent
	{
		std::vector<unsigned int> MeshIds;
		bool Instanced;
	};

	struct CW_API SkyboxComponent : SingletonComponent
	{
		SkyboxComponent()
		{
			std::cout << "skybox created\n";
		}

		std::vector<float> Vertices;
		unsigned int IrradianceMap;
		unsigned int BrdfLutMap;
		unsigned int PrefilterMap;
		Shader Shader;
		unsigned int Vao;
		unsigned int TextureId;
	};

	struct CW_API CameraComponent : SingletonComponent
	{
		glm::vec3 Position;
		glm::vec3 Forward;
		glm::vec3 Up;

		int Width;
		int height;
		float FoV;
		float speed;
		float sensitivity;
		float Yaw = 0;
		float Pitch = 0;
		float Near;
		float Far;

		// TODO: move this to system
		glm::mat4 CameraMatrix()
		{
			glm::mat4 view = glm::mat4(1);
			glm::mat4 projection = glm::mat4(1);

			view = glm::lookAt(Position, Position + Forward, Up);
			projection = glm::perspective(glm::radians(FoV), (float)Width / height, Near, Far);

			auto right = glm::normalize(glm::cross(Forward, Up));

			auto result = projection * view;

			return result;
		}

		glm::mat4 Projection()
		{
			glm::mat4 projection = glm::mat4(1);
			projection = glm::perspective(glm::radians(FoV), (float)Width / height, Near, Far);
			return projection;
		}

		glm::mat4 View()
		{
			glm::mat4 view = glm::mat4(1);
			return glm::lookAt(Position, Position + Forward, Up);
		}

	};

	struct CW_API MeshComponent
	{
		MeshComponent()
		{
			std::cout << "mesh\n";
		}

		std::vector<Vertex> Vertices;
		std::vector<Texture> Textures;
		std::vector<unsigned int> Indices;
		unsigned int VaoId;
		unsigned int instanceVbo;
		Shader Shader;
		AABB AABB;
		vec3 Color = vec3(1);

		void Setup()
		{
			VAO vao;
			vao.Bind();

			EBO ebo(Indices);
			VBO vbo(Vertices);

			VaoId = vao.Id;

			unsigned int stride = sizeof(Vertex) / sizeof(float);

			vao.LinkAttribArray<float>(vbo, 0, 3, GL_FLOAT, stride, 0);
			vao.LinkAttribArray<float>(vbo, 1, 3, GL_FLOAT, stride, 3);
			vao.LinkAttribArray<float>(vbo, 2, 2, GL_FLOAT, stride, 6);
			vao.LinkAttribArray<float>(vbo, 3, 3, GL_FLOAT, stride, 8);
			vao.LinkAttribArray<float>(vbo, 4, 3, GL_FLOAT, stride, 11);

			vao.Unbind();
			vbo.Unbind();
			ebo.Unbind();

			float minX = std::numeric_limits<float>::max();
			float minY = minX;
			float minZ = minX;
			
			float maxX = std::numeric_limits<float>::min();
			float maxY = maxX;
			float maxZ = maxX;

			for (const Vertex& vertex : Vertices)
			{
				maxX = std::max(maxX, vertex.Position.x);
				minX = std::min(minX, vertex.Position.x);
				maxY = std::max(maxY, vertex.Position.y);
				minY = std::min(minY, vertex.Position.y);
				maxZ = std::max(maxZ, vertex.Position.z);
				minZ = std::min(minZ, vertex.Position.z);
			}
			AABB = { maxX,minX,maxY,minY,maxZ,minZ };
		}

		void MakeInstanced(std::vector<glm::mat4>& transforms)
		{
			glBindVertexArray(VaoId);

			VBO instanceVBO(transforms);


			unsigned int stride = sizeof(glm::mat4) / sizeof(float);

			instanceVBO.Bind();
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));


			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);

			glBindVertexArray(0);
			instanceVBO.Unbind();
		}
	};


}