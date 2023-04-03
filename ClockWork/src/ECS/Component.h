#pragma once

#include "Graphics/Mesh.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "glm.hpp"

#include <typeinfo>

using namespace glm;

namespace CW
{
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
			glm::mat quatMatrix = glm::toMat4(rotation);
			model *= quatMatrix;
			model = glm::scale(model, Scale);
			return model;
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


	struct CW_API CameraComponent
	{
		glm::vec3 Position;
		glm::vec3 Forward;
		glm::vec3 Up;

		int Width;
		int height;
		float FoV;
		float speed;
		float sensitivity;
		float Yaw;
		float Pitch;

		// TODO: move this to system
		glm::mat4 CameraMatrix()
		{
			glm::mat4 view = glm::mat4(1);
			glm::mat4 projection = glm::mat4(1);

			view = glm::lookAt(Position, Position + Forward, Up);
			projection = glm::perspective(glm::radians(FoV), (float)Width / height, 0.1f, 100.0f);

			auto right = glm::normalize(glm::cross(Forward, Up));

			return projection * view;
		}

		glm::mat4 ViewProj()
		{
			glm::mat4 view = glm::mat4(1);
			glm::mat4 projection = glm::mat4(1);

			view = glm::lookAt(Position, Position + Forward, Up);
			projection = glm::perspective(glm::radians(FoV), (float)Width / height, 0.1f, 100.0f);

			auto right = glm::normalize(glm::cross(Forward, Up));

			return view * projection;
		}

		glm::mat4 ViewMatrix()
		{
			glm::mat4 view = glm::mat4(1);
			return glm::lookAt(Position, Position + Forward, Up);
		}

	};


}