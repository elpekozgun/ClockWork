#pragma once
#include "../ECS.h"
#include "../Component/Component.h"
#include "../ISystem.h"
#include "glm.hpp"
#include "Core/Event.h"
#include <iostream>

using namespace glm;

namespace CW
{
	class CW_API PseudoPhysicsSystem : public ISystem
	{
	public:
		void Update(float dt) override;
		void GetInput(int input) override;

		Event<float> OnUpdated;

	private:
		float _updateRate = 50;
		float _currentDelta = 0;
	};
}