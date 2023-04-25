#include "CollisionSystem.h"


namespace CW
{
	void CollisionSystem::Update(float dt)
	{
		ComputeCollisions();
	}

	void CollisionSystem::ComputeCollisions()
	{
		auto aabbs = _ecs->GetComponentArray<AABBComponent>();
		auto transforms = _ecs->GetComponentArray<TransformComponent>();

		std::vector<GPUBox> aabbData;
		aabbData.reserve(_entities.size());

		for (const auto& entity : _entities)
		{
			const auto& aabb = aabbs->GetData(entity);
			const auto& transform = transforms->GetData(entity);
			const auto transformedAABB = TransformAABB(aabb, transform);

			//aabbData.emplace_back(GPUBox{ glm::vec4(transformedAABB.Min,0), glm::vec4(transformedAABB.Max,0) });
			aabbData.emplace_back(GPUBox{ transformedAABB.Min, entity, transformedAABB.Max, 0});
		}

		const auto size = static_cast<unsigned int>(aabbData.size());

		// Upload the boxes to the GPU
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _boxBuffer1);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(GPUBox), aabbData.data(), GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _boxBuffer1);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _boxBuffer2);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(GPUBox), aabbData.data(), GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _boxBuffer2);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _collisionBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size * size * sizeof(GPUCollision), NULL, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, _collisionBuffer);

		// Bind atomic counter
		unsigned int collisionCount = 0;
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterBuffer);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(unsigned int), &collisionCount, GL_DYNAMIC_READ);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, _atomicCounterBuffer);

		const auto groups = (size * (size - 1) / 2 + 63) / 64;

		CollisionCompute.Use();
		CollisionCompute.Dispatch(groups , 1, 1, GL_ALL_BARRIER_BITS);

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterBuffer);
		glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(unsigned int), &collisionCount);

		if (collisionCount > 0)
		{
			std::vector<GPUCollision> collisions(collisionCount);

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _collisionBuffer);
			auto collisionDataPtr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, collisionCount * sizeof(GPUCollision), GL_MAP_READ_BIT);
			if (collisionDataPtr != nullptr)
				std::memcpy(collisions.data(), collisionDataPtr, collisionCount * sizeof(GPUCollision));
			
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

			//auto cols = _ecs->GetSingletonComponent<CollisionDataSourceComponent>();
			//cols->collisions = collisions;


			for(int i = 0; i < collisionCount; i++)
			{
				auto collision = collisions[i];

				if (collision.Id1 == 0)
				{
					auto& point = collision.Point;

					auto& transform = transforms->GetData(collision.Id1);
					auto& transform2 = transforms->GetData(collision.Id2);
					
					// collision.Point is actually the penetration amount of 2 boxes colliding.
					auto dot = glm::dot(transform2.Position - transform.Position, collision.Point);

					if (dot > 0)
					{
						transform.Position -= collision.Point;
					}
					else
					{
						transform.Position += collision.Point;
					}
				}
			}
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	}

	AABBComponent CollisionSystem::TransformAABB(const AABBComponent& aabb, const TransformComponent& transform) 
	{
		// Compute the transformed min and max values for the AABB
		glm::vec3 min = transform.Position + aabb.Min * transform.Scale;
		glm::vec3 max = transform.Position + aabb.Max * transform.Scale;

		return AABBComponent(min, max);
	}
}
