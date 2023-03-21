#pragma once

#include <queue>
#include <array>
#include "EcsType.h"

class CW_API EntityManager
{
public:
	EntityManager() 
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
		{
			_availableEntities.push(entity);
		}
	}

	Entity CreateEntity()
	{
		Entity id = _availableEntities.front();
		_availableEntities.pop();
		_activeEntities++;

		return id;
	}

	void DestroyEntity(Entity entity)
	{
		_entityMasks[entity].reset();
		_availableEntities.push(entity);
		_activeEntities--;
	}

	void Setmask(Entity entity, Mask mask)
	{
		_entityMasks[entity] = mask;
	}

	Mask GetMask(Entity entity)
	{
		return _entityMasks[entity];
	}



private:
	std::queue<Entity> _availableEntities;
	std::array<Mask, MAX_ENTITIES> _entityMasks;
	uint32_t _activeEntities = 0;
};