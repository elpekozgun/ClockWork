#include "PseudoPhysicsSystem.h"


void PseudoPhysicsSystem::Update(float dt)
{
	ECS& instance = ECS::Instance();
	for (auto& entity : _entities)
	{
			auto& transform = instance.GetComponent<Transform>(entity);
			auto& rigidBody = instance.GetComponent<RigidBody>(entity);
			const auto& gravity = instance.GetComponent<Gravity>(entity);

			transform.position += rigidBody.velocity * dt;
			rigidBody.velocity += gravity._force * dt;
	}
}
