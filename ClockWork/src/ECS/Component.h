#pragma once

#include <glm.hpp>

#include "Core/Core.h"

using namespace glm;

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