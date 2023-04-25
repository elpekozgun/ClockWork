#include "CollisionSystem.h"


namespace CW
{
	void CollisionSystem::Update(float dt)
	{
		ComputeCollisions();
		return;
		/*auto aabbs = _ecs->GetComponentArray<AABBComponent>();
		auto transforms = _ecs->GetComponentArray<TransformComponent>();

		std::unordered_set<EntityId> destroyed;

		for (auto& entity1 : _entities)
		{
			auto& aabb1 = aabbs->GetData(entity1);
			auto& transform1 = transforms->GetData(entity1);
			for (auto& entity2 : _entities)
			{
				if (entity1 != entity2)
				{
					auto aabb2 = aabbs->GetData(entity2);
					auto& transform2 = transforms->GetData(entity2);

					if (CheckAABBCollision(aabb1, transform1, aabb2, transform2))
					{
						bool isPlayer = _ecs->HasComponent<Player>(entity1);
						if (isPlayer)
						{
							destroyed.insert(entity2);
						}
						else
						{
							isPlayer = _ecs->HasComponent<Player>(entity2);
							if (isPlayer)
							{
								destroyed.insert(entity1);
							}
						}
					}
				}
			}
		}

		for (auto entity: destroyed)
		{
			_ecs->DestroyEntity(entity);
		}*/
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

		// Upload the collision data to the GPU
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _collisionBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size * size * sizeof(vec2), NULL, GL_DYNAMIC_DRAW);
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
			std::vector<vec2> output(collisionCount);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, _collisionBuffer);
			auto pCollisionData = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, collisionCount * sizeof(vec2), GL_MAP_READ_BIT);
			if (pCollisionData != nullptr)
			{
				std::memcpy(output.data(), pCollisionData, collisionCount * sizeof(vec2));
			}
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

			for (const auto& collision : output)
			{
				if (collision.x == 0)
				{
					auto id = (unsigned int)(collision.y);
					_ecs->DestroyEntity(id);
				}
			}
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	}



	bool CollisionSystem::CheckAABBCollision(const AABBComponent& aabb1, const TransformComponent& transform1, const AABBComponent& aabb2, const TransformComponent& transform2) {
		// Compute the transformed AABBs for both entities
		AABBComponent transformedAABB1 = TransformAABB(aabb1, transform1);
		AABBComponent transformedAABB2 = TransformAABB(aabb2, transform2);

		if (transformedAABB1.Min.x < transformedAABB2.Min.x || transformedAABB1.Min.x > transformedAABB2.Max.x) 
			return false;

		if (transformedAABB1.Min.y < transformedAABB2.Min.y || transformedAABB1.Min.y > transformedAABB2.Max.y)
			return false;
		
		if (transformedAABB1.Min.z < transformedAABB2.Min.z || transformedAABB1.Min.z > transformedAABB2.Max.z)
			return false;

		return true;
	}

	AABBComponent CollisionSystem::TransformAABB(const AABBComponent& aabb, const TransformComponent& transform) 
	{
		// Compute the transformed min and max values for the AABB
		glm::vec3 min = transform.Position + aabb.Min * transform.Scale;
		glm::vec3 max = transform.Position + aabb.Max * transform.Scale;

		return AABBComponent(min, max);
	}
}
