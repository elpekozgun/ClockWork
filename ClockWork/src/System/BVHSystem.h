#pragma once

#include "ECS/ECS.h"

namespace CW
{
	class CW_API BVHSystem : public ISystem
	{
		void Update(float dt) override
		{
			auto transforms = _ecs->GetComponentArray<AABBComponent>();

			for (auto& entity : _entities)
			{
				auto& transform = transforms->GetData(entity);
			}


		}

		void UpdateBVHNode(BVHNode* node)
		{
			if (node->Left && node->Right)
			{
				node->aabb = CombineAABBs(node->Left->aabb, node->Right->aabb);
			}
			else if (node->Entity != -1)
			{
				 
			}
		}

		AABB CombineAABBs(AABB& left, AABB& right)
		{
			AABB combined;
			combined.minX = left.minX < right.minX ? left.minX : right.minX;
			combined.minY = left.minY < right.minY ? left.minY : right.minY;
			combined.minZ = left.minZ < right.minZ ? left.minZ : right.minZ;
			combined.maxX = left.maxX > right.maxX ? left.maxX : right.maxX;
			combined.maxY = left.maxY > right.maxY ? left.maxY : right.maxY;
			combined.maxZ = left.maxZ > right.maxZ ? left.maxZ : right.maxZ;
			return combined;
		}

	};
}