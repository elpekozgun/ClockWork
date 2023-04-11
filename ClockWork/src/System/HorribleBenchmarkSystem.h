#pragma once

#include "ECS/ECS.h"

namespace CW
{
	// go for naive approach for now.
	class CW_API HorribleBenchmarkSystem : public ISystem
	{
	public:
		void Update(float dt) override;
	private:
		bool CheckAABBCollision(const AABBComponent& aabb1, const TransformComponent& transform1, const AABBComponent& aabb2, const TransformComponent& transform2);
		AABBComponent TransformAABB(const AABBComponent& aabb, const TransformComponent& transform);

	};



}