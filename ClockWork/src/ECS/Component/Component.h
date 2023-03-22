#include "Core/Core.h"
#include <glm.hpp>

using namespace glm;

namespace CW
{
	struct CW_API Gravity
	{
		vec3 _force;
	};

	struct CW_API RigidBody
	{
		vec3 velocity;
		vec3 acceleration;
	};

	struct CW_API Transform
	{
		vec3 position;
		vec3 rotation;
		vec3 scale;
	};
}