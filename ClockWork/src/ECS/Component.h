#pragma once

#include <glm.hpp>

#include "Core/Core.h"
#include <typeinfo>

using namespace glm;
struct CW_API IComponent;

namespace CW
{
	struct CW_API GravityComponent 
	{
		vec3 _force;
	};

	struct CW_API RigidBodyComponent 
	{
		vec3 velocity;
		vec3 acceleration;
	};

	struct CW_API TransformComponent 
	{
		vec3 position;
		vec3 rotation;
		vec3 scale;
	};

	struct CW_API RendererComponent 
	{
		
	};

}