#include "PlayerMovement.h"

void PlayerController::Update(float deltaTime)
{
	for (auto& entity : _entities)
	{
		auto input = CW::ECS::Instance().GetComponent<InputComponent>(entity);
		auto transform = CW::ECS::Instance().GetComponent<TransformComponent>(entity);
		
		transform.Position += glm::vec3(input.Right - input.Left, input.Up - input.Down, 0);

		std::cout << "entityId: " << entity
			<< " position X : " << transform.Position.x
			<< " position Y : " << transform.Position.y
			<< " position Z : " << transform.Position.z << "\n";
	}

}

void PlayerController::GetInput(int input)
{

}

void InputSystem::Update(float deltaTime)
{

}

void InputSystem::GetInput(int input)
{
}
