#include "PlayerController.h"

void PlayerController::Update(float deltaTime)
{
	auto& input = InputState::Instance();
	for (auto& entity : _entities)
	{
		glm::vec3 movement(0,0,0);

		if (input.IsKeyDown(CW::KEY_UP))
		{
			movement.y += deltaTime;
		}

		if (input.IsKeyDown(CW::KEY_DOWN))
		{
			movement.y -= deltaTime;
		}

		if (input.IsKeyDown(CW::KEY_RIGHT))
		{
			movement.x += deltaTime;
		}

		if (input.IsKeyDown(CW::KEY_LEFT))
		{
			movement.x -= deltaTime;
		}

		auto transform = CW::ECS::Instance().GetComponent<TransformComponent>(entity);

		transform.Position += movement;

		if (glm::length(movement) != 0)
		{
			std::cout << "entityId: " << entity
				<< " position X : " << transform.Position.x
				<< " position Y : " << transform.Position.y
				<< " position Z : " << transform.Position.z << "\n";
		}
		
		CW::ECS::Instance().SetComponent(entity, transform);

	}

}

void InputSystem::Update(float deltaTime)
{
	
}

