#pragma once
#include "glm.hpp"
#include "gtc/quaternion.hpp"
#include "assimp/BaseImporter.h"
#include <vector>
#include <string>

namespace CW
{
	struct KeyPosition
	{
		glm::vec3 position;
		float timeStamp;
	};

	struct KeyRotation
	{
		glm::quat orientation;
		float timeStamp;
	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;
	};


}