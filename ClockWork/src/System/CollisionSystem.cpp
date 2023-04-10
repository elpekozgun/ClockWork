#include "CollisionSystem.h"

namespace CW
{
	void CollisionSystem::Update(float dt)
	{
		auto aabbs = _ecs->GetComponentArray<AABBComponent>();
		auto transforms = _ecs->GetComponentArray<TransformComponent>();

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
						//bool isPlayer = _ecs->HasComponent<Player>(entity1);
						//if (isPlayer)
						//{
						//	_ecs->DestroyEntity(entity2);
						//}
						//else
						//{
						//	isPlayer = _ecs->HasComponent<Player>(entity2);
						//	if (isPlayer)
						//	{
						//		_ecs->DestroyEntity(entity1);
						//	}
						//}
					}
				}
			}
		}
	}

	bool CollisionSystem::CheckAABBCollision(const AABBComponent& aabb1, const TransformComponent& transform1, const AABBComponent& aabb2, const TransformComponent& transform2) {
		// Compute the transformed AABBs for both entities
		AABBComponent transformedAABB1 = TransformAABB(aabb1, transform1);
		AABBComponent transformedAABB2 = TransformAABB(aabb2, transform2);

		// Check for overlap in each dimension
		bool overlapX = (transformedAABB1.Min.x <= transformedAABB2.Max.x) && (transformedAABB1.Max.x >= transformedAABB2.Min.x);
		bool overlapY = (transformedAABB1.Min.y <= transformedAABB2.Max.y) && (transformedAABB1.Max.y >= transformedAABB2.Min.y);
		bool overlapZ = (transformedAABB1.Min.z <= transformedAABB2.Max.z) && (transformedAABB1.Max.z >= transformedAABB2.Min.z);

		// If there is overlap in all dimensions, there is a collision
		return overlapX && overlapY && overlapZ;
	}

	AABBComponent CollisionSystem::TransformAABB(const AABBComponent& aabb, const TransformComponent& transform) {
		// Compute the transformed min and max values for the AABB
		glm::vec3 min = transform.Position + aabb.Min * transform.Scale;
		glm::vec3 max = transform.Position + aabb.Max * transform.Scale;

		return AABBComponent(min, max);
	}
}
