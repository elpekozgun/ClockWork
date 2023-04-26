#pragma once
#include <math.h>

#include "Graphics/ShaderFactory.h"
#include "ECS/ECS.h"
#include "Core/Macro.h"

namespace CW
{
	class CW_API CollisionSystem : public ISystem
	{
	public:

		CollisionSystem()
		{
			CollisionCompute = ShaderFactory::CreateComputeShader("collisionCompute", R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Collision.comp)");
            
			glGenBuffers(1, _atomicCounterBuffer);
			glGenBuffers(2, _collisionBuffers);
			glGenBuffers(2, _boxBuffers);
			Name = "CollisionSystem";

			_updateRate = 0.02f;
		}

		void Update(float dt) override;
	private:
		AABBComponent TransformAABB(const AABBComponent& aabb, const TransformComponent& transform);

		void ComputeCollisions();

		ComputeShader CollisionCompute;

		// use double buffering to optimize read/writes per frame
		GLuint _boxBuffers[2];
		GLuint _collisionBuffers[2]; 
		GLuint _atomicCounterBuffer[2];

		GLuint _activeBuffer = 0;
	};

	// align to 16bytes per std430
	struct CW_API GPUBox
	{
		vec3 Min;
		unsigned int Id;
		vec3 Max;
		float padding;
	};

	struct CW_API GPUCollision
	{
		vec3 Point;
		unsigned int Id1;
		vec3 Normal;
		unsigned int Id2;
	};

}