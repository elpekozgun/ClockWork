#include "PlayerController.h"

void PlayerController::Update(float dt)
{
	auto& input = Input::Instance();
	for (auto& entity : _entities)
	{
		dt = 0.01f;
		auto& player = _ecs->GetComponent<CW::Player>(entity);
		auto& transform = _ecs->GetComponent<CW::TransformComponent>(entity);

		float horizontal = 0.0f;
		float vertical = 0.0f;
		float rotational = 0.0f;

		if (input.GetKeyDown(CW::KEY_W))
		{
			vertical = 1.0f;
		}

		if (input.GetKeyDown(CW::KEY_S))
		{
			vertical = -1.0f;
		}

		if (input.GetKeyDown(CW::KEY_Q))
		{
			horizontal = 1.0f;
		}

		if (input.GetKeyDown(CW::KEY_E))
		{
			horizontal = -1.0f;
		}

		if (input.GetKeyDown(CW::KEY_A))
		{
			rotational = -1.0f;
		}

		if (input.GetKeyDown(CW::KEY_D))
		{
			rotational = 1.0f;
		}

		float rotationAmount = rotational * player.turnspeed * dt;
		transform.Rotation += glm::vec3(0, rotationAmount, 0);

		vec3 forward = transform.Forward();
		
		glm::vec3 movement = forward * vertical * player.moveSpeed ;
		glm::vec3 strafe = glm::cross(forward, glm::vec3(0, 1, 0)) * horizontal * player.moveSpeed ;

		// Set transform
		transform.Position += (movement + strafe) * dt ;
		
	}
}


