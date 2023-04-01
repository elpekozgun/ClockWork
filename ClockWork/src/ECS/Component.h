#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Graphics/Mesh.h"

#include "Core/Core.h"
#include <typeinfo>


using namespace glm;
struct CW_API IComponent;

namespace CW
{
	struct CW_API TransformComponent
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
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

	};


	struct CW_API PhysicsComponent
	{
		vec3 Acceleration;
		vec3 Velocity;
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

		glm::mat4 CameraMatrix()
		{
			glm::mat4 view = glm::mat4(1);
			glm::mat4 projection = glm::mat4(1);

			view = glm::lookAt(Position, Position + Forward, Up);
			projection = glm::perspective(glm::radians(FoV), (float)Width / height, 0.1f, 100.0f);

			auto right = glm::normalize(glm::cross(Forward, Up));

			return projection * view;
		}

	};


}