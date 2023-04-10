#pragma once
#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "Core/Input.h"

namespace CW
{
	class CW_API PlayerController : public CW::ISystem
	{
		virtual void Update(float dt) override;
	};
}