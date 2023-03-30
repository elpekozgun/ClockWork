#include "CameraSystem.h"


namespace CW
{
	void CameraSystem::Update(float dt)
	{
		auto& input = InputState::Instance();
		auto& ecs = ECS::Instance();
		
		for (auto& entity : _entities)
		{
			auto& camera = ecs.GetComponent<CameraComponent>(entity);
			auto right = glm::normalize(glm::cross(camera.Forward, camera.Up));

			if (input.IsKeyDown(CW::KEY_W))
			{
				camera.Position += camera.speed * camera.Forward * dt ;
			}
			if (input.IsKeyDown(CW::KEY_A))
			{
				camera.Position -= camera.speed * right * dt;
			}
			if (input.IsKeyDown(CW::KEY_S))
			{
				camera.Position -= camera.speed * camera.Forward * dt;
			}
			if (input.IsKeyDown(CW::KEY_D))
			{
				camera.Position += camera.speed * right * dt;
			}
			if (input.IsKeyDown(CW::KEY_SPACE))
			{
				camera.Position += camera.speed * camera.Up * dt;
			}
			if (input.IsKeyDown(CW::KEY_LEFT_SHIFT))
			{
				camera.Position -= camera.speed * camera.Up * dt;
			}
		}
	}

}
