#pragma once

#include "Graphics/ShaderFactory.h"
#include "ECS/ECS.h"

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
			         
		}

		void Update(float dt) override;
	private:
		bool CheckAABBCollision(const AABBComponent& aabb1, const TransformComponent& transform1, const AABBComponent& aabb2, const TransformComponent& transform2);
		AABBComponent TransformAABB(const AABBComponent& aabb, const TransformComponent& transform);

		void ComputeCollisions();

		ComputeShader CollisionCompute;

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

}