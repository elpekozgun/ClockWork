#pragma once

#include "Core/Core.h"
#include "glm.hpp"
#include "Core/Defines.h"

namespace CW
{
	struct CW_API Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UV;
		glm::vec3 Tangent;
		int BoneIds[MAX_BONE_INFLUENCE];
		float Weights[MAX_BONE_INFLUENCE];
	};
}