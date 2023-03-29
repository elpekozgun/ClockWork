#include "PlayerController.h"

void PlayerController::Update(float dt)
{
	auto& input = InputState::Instance();
	for (auto& entity : _entities)
	{
		glm::vec3 movement(0,0,0);

		if (input.IsKeyDown(CW::KEY_UP))
		{
			movement.y += dt;
		}

		if (input.IsKeyDown(CW::KEY_DOWN))
		{
			movement.y -= dt;
		}

		if (input.IsKeyDown(CW::KEY_RIGHT))
		{
			movement.x += dt;
		}

		if (input.IsKeyDown(CW::KEY_LEFT))
		{
			movement.x -= dt;
		}

		auto& transform = CW::ECS::Instance().GetComponent<CW::TransformComponent>(entity);

		transform.Position += movement;

		if (glm::length(movement) != 0)
		{
			std::cout << "entityId: " << entity
				<< " position X : " << transform.Position.x
				<< " position Y : " << transform.Position.y
				<< " position Z : " << transform.Position.z << "\n";
		}
		
		//CW::ECS::Instance().SetComponent(entity, transform);

	}

}


