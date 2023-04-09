#include "PhysicsSystem.h"

namespace CW
{

	void PhysicsSystem::Update(float dt)
	{
		auto& input = Input::Instance();

		std::vector<EntityId> toBeDestroyed;

		auto array1 = _ecs->GetComponentArray<PhysicsComponent>();
		auto array2 = _ecs->GetComponentArray<TransformComponent>();

		for (auto& entity : _entities)
		{
			auto& physics = array1->GetData(entity);
			auto& transform = array2->GetData(entity);

			//physics.Acceleration -= glm::vec3(0,0.00981f,0);

			physics.Velocity += physics.Acceleration * dt;
			transform.Position += physics.Velocity * dt;

			//if (abs(transform.Position.x) > 40.0f || abs(transform.Position.z) > 40.0f)
			//{
			//	_ecs->DestroyEntity(entity); // weird stuff happening maybe destroy entities at the end of the frame?
			//}
		}
	}
	
}