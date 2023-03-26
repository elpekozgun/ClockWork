#include "PseudoPhysicsSystem.h"

namespace CW
{
	void PseudoPhysicsSystem::Update(float dt)
	{
		_currentDelta += dt;
		if (_currentDelta >= _updateRate)
		{
			ECS& instance = ECS::Instance();
			for (auto& entity : _entities)
			{
				auto& transform = instance.GetComponent<TransformComponent>(entity);
				auto& rigidBody = instance.GetComponent<RigidBodyComponent>(entity);
				const auto& gravity = instance.GetComponent<GravityComponent>(entity);

				transform.position += rigidBody.velocity * dt;
				rigidBody.velocity += gravity._force * dt;
			}
			OnUpdated.Invoke(_currentDelta);
			_currentDelta = 0;
		}
	}

	void PseudoPhysicsSystem::GetInput(int input)
	{
		if (input == CW::KEY_T)
		{
			std::cout << "T pressed" << std::endl;
		}
	}

}