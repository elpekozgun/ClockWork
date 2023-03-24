#pragma once
#include <map>
#include "ECS/Entity/Entity.h"

namespace CW
{
	class SceneGraph
	{
	private:
		std::vector<std::shared_ptr<Entity>> _sceneEntities;
	};
}