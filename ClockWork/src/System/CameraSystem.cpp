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
			if (input.IsMouseDown(CW::BUTTON_RIGHT))
			{
				float xOffset = input.MouseDX * camera.sensitivity;
				float yOffset = input.MouseDY * camera.sensitivity;

				camera.Yaw += xOffset;
				camera.Pitch += yOffset;

				if (camera.Pitch > 89.0f) 
					camera.Pitch = 89.0f;
				if (camera.Pitch < -89.0f) 
					camera.Pitch = -89.0f;

				glm::vec3 forward;
				forward.x = cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
				forward.y = sin(glm::radians(camera.Pitch));
				forward.z = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
				
				camera.Forward = glm::normalize(forward);
				right = glm::normalize(glm::cross(camera.Forward, glm::vec3(0,1,0)));
				camera.Up = glm::normalize(glm::cross(right, camera.Forward));
			}
		}
	}

}
