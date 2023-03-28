#pragma once

#include "ECS/ISystem.h"
#include "glad/glad.h"

namespace CW
{
	class CW_API RenderSystem : public ISystem
	{
		virtual void Update(float deltaTime) override;
	};
}
