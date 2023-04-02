#include "PlayerController.h"

void PlayerController::Update(float dt)
{
	auto& input = InputState::Instance();
	for (auto& entity : _entities)
	{
		auto& player = CW::ECS::Instance().GetComponent<CW::Player>(entity);
		auto& transform = CW::ECS::Instance().GetComponent<CW::TransformComponent>(entity);

		float horizontal = 0.0f;
		float vertical = 0.0f;

		if (input.IsKeyDown(CW::KEY_W))
		{
			vertical = 1.0f;
		}

		if (input.IsKeyDown(CW::KEY_S))
		{
			vertical = -1.0f;
		}

		if (input.IsKeyDown(CW::KEY_D))
		{
			horizontal = 1.0f;
		}

		if (input.IsKeyDown(CW::KEY_A))
		{
			horizontal = -1.0f;
		}

		// move character
		glm::vec3 direction = glm::vec3(horizontal, 0, vertical);

		glm::quat rotation = glm::quat(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z));
		glm::mat4 rotationMatrix = glm::toMat4(rotation);
		
		glm::vec3 forward = glm::normalize(glm::vec3(rotationMatrix[2]));

		glm::vec3 movement = forward * direction.z * player.moveSpeed;
		glm::vec3 strafe = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0))) * direction.x * player.moveSpeed;

		transform.Position += (movement + strafe) * dt;

		// rotate character
		transform.Rotation.y -= horizontal * player.turnspeed * dt; //forward.y + horizontal * player.turnspeed * dt;

	}
}


