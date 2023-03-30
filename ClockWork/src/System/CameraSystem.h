#pragma once

#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "Core/InputState.h"
#include "gtc/matrix_transform.hpp"

namespace CW
{
	class CW_API CameraSystem : public ISystem
	{
		virtual void Update(float dt) override;
	};
}