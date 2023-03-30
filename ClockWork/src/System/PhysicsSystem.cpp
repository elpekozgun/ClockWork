#include "PhysicsSystem.h"

void CW::PhysicsSystem::Update(float dt)
{
	auto& input = InputState::Instance();
	auto& ecs = ECS::Instance();

	if (input.IsKeyDown(CW::KEY_A))
	{
		for (auto& entity : _entities)
		{
			auto& physics = ecs.GetComponent<PhysicsComponent>(entity);
			auto& transform = ecs.GetComponent<TransformComponent>(entity);

			physics.Velocity += physics.Acceleration * dt;
			transform.Position += physics.Velocity * dt;

			//std::cout << "entity: " << entity <<
			//	" - new velocity: " << physics.Velocity.x << ", " << physics.Velocity.y << ", " << physics.Velocity.z <<
			//	" - new position: " << transform.Position.x << ", " << transform.Position.y << ", " << transform.Position.z << "\n";

		}
	}
}
