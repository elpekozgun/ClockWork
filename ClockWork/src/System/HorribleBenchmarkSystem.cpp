#include "HorribleBenchmarkSystem.h"

namespace CW
{
	void HorribleBenchmarkSystem::Update(float dt)
	{
		auto aabbs = _ecs->GetComponentArray<AABBComponent>();
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

		for (auto entity : destroyed)
		{
			_ecs->DestroyEntity(entity);
		}
	}

	bool HorribleBenchmarkSystem::CheckAABBCollision(const AABBComponent& aabb1, const TransformComponent& transform1, const AABBComponent& aabb2, const TransformComponent& transform2) {
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

	AABBComponent HorribleBenchmarkSystem::TransformAABB(const AABBComponent& aabb, const TransformComponent& transform) {
		// Compute the transformed min and max values for the AABB
		glm::vec3 min = transform.Position + aabb.Min * transform.Scale;
		glm::vec3 max = transform.Position + aabb.Max * transform.Scale;

		return AABBComponent(min, max);
	}
}
