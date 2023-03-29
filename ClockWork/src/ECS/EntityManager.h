#pragma once

#include <queue>
#include <array>
#include "Core/Defines.h"

namespace CW
{
	class CW_API EntityManager
	{
	public:
		EntityManager()
		{
			for (int i = 0; i < MAX_ENTITY; i++)
			{
				_availableEntities.push(i);
			}
		}

		EntityId CreateEntity()
		{
			EntityId id = _availableEntities.front();
			_availableEntities.pop();
			_entities++;

			return id;
		}

		void DestroyEntity(EntityId entity)
		{
			_entityMasks[entity].reset();
			_availableEntities.push(entity);
			_entities--;
		}

		void Setmask(EntityId entity, ComponentMask mask)
		{
			_entityMasks[entity] = mask;
		}

		ComponentMask GetMask(EntityId entity)
		{
			return _entityMasks[entity];
		}



	private:
		std::queue<EntityId> _availableEntities;
		
		ComponentMask _entityMasks[MAX_ENTITY];
		uint32_t _entities = 0;
	};
}