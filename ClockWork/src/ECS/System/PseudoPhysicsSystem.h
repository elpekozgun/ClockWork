#pragma once
#include "ECS/Core/EcsType.h"
#include "glm.hpp"
#include "ECS/Core/ECS.h"

using namespace glm;

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



class CW_API PseudoPhysicsSystem : public System
{
public:
	void Update(float dt) override;
};
