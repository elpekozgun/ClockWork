#pragma once
#include <math.h>

#include "Graphics/ShaderFactory.h"
#include "ECS/ECS.h"
#include "Core/Macro.h"

namespace CW
{
	// go for naive approach for now.
	class CW_API CollisionSystem : public ISystem
	{
	public:

		CollisionSystem()
		{
			CollisionCompute = ShaderFactory::CreateComputeShader("collisionCompute", R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Collision.comp)");
            
            glGenBuffers(1, &_boxBuffer1);
            glGenBuffers(1, &_boxBuffer2);
			glGenBuffers(1, &_collisionBuffer);
			glGenBuffers(1, &_atomicCounterBuffer);
			Name = "CollisionSystem";

			_updateRate = 0.02f;
		}

		void Update(float dt) override;
	private:
		AABBComponent TransformAABB(const AABBComponent& aabb, const TransformComponent& transform);

		void ComputeCollisions();

		ComputeShader CollisionCompute;

		//std::vector<vec2> collisionPairs;
		//std::vector<float> collisionPoints;

        GLuint _boxBuffer1;
		GLuint _boxBuffer2;
        GLuint _collisionBuffer;
		GLuint _atomicCounterBuffer;
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