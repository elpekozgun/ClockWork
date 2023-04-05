#pragma once

#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "Core/Input.h"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_angle.hpp"


namespace CW
{
	class CW_API CameraSystem : public ISystem
	{
		virtual void Update(float dt) override;
	};
}