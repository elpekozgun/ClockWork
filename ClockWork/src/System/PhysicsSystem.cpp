#include "PhysicsSystem.h"

void CW::PhysicsSystem::Update(float dt)
{
	auto& input = Input::Instance();

	std::vector<EntityId> toBeDestroyed;

	for (auto& entity : _entities)
	{
		auto& physics = _ecs->GetComponent<PhysicsComponent>(entity);
		auto& transform = _ecs->GetComponent<TransformComponent>(entity);

		//physics.Acceleration -= glm::vec3(0,0.00981f,0);
		 
		physics.Velocity += physics.Acceleration * dt;
		transform.Position += physics.Velocity * dt;

		//if (abs(transform.Position.x) > 40.0f || abs(transform.Position.z) > 40.0f)
		//{
		//	ecs.DestroyEntity(entity); // weird stuff happening maybe destroy entities at the end of the frame?
		//}
	}
}
