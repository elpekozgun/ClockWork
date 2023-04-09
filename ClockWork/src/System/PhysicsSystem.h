
#pragma once
#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "Core/Input.h"
#include <thread>

namespace CW
{
	class CW_API PhysicsSystem : public ISystem
	{
		virtual void Update(float dt) override;

	};
}