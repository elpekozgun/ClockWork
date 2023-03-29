#pragma once

#include <glm.hpp>

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
	
	/*struct CW_API Renderable
	{
		Mesh Mesh;
		Material Material;
	};*/

	struct CW_API Player
	{

	};


	struct CW_API PhysicsComponent
	{
		vec3 Acceleration;
		vec3 Velocity;
	};

}