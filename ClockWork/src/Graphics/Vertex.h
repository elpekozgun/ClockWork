#pragma once

#include "Core/Core.h"
#include "glm.hpp"


struct CW_API Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int BoneIds[4];
	float Weights[4];
	// we might not need this at all, as we can compute it on shader.
	// do a benchmark on both, because it will increase the size of mesh data in componentArray which
	// needs to be retrieved
};