#include "PhysicsSystem.h"

#include <iostream>

using namespace std;

namespace CW
{
	void PhysicsSystem::Update(float deltaTime)
	{
		_deltaTime += deltaTime;
		if (_deltaTime >= _updateRate)
		{
			OnUpdated.Invoke(_deltaTime);
			_deltaTime = 0;
		}
	}

	float PhysicsSystem::GetUpdateRate() const
	{
		return _deltaTime;
	}

	void PhysicsSystem::Shutdown() const
	{

	}
}