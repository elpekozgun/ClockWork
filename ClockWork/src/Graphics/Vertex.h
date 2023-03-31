#pragma once

#include "Core/Core.h"
#include "glm.hpp"


struct CW_API Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 Color;
	glm::vec2 UV;
};