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
			for (EntityId entity = 0; entity < MAX_ENTITY; entity++)
			{
				_availableEntities.push(entity);
			}
		}

		EntityId CreateEntity()
		{
			EntityId id = _availableEntities.front();
			_availableEntities.pop();
			_activeEntities++;

			return id;
		}

		void DestroyEntity(EntityId entity)
		{
			_entityMasks[entity].reset();
			_availableEntities.push(entity);
			_activeEntities--;
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
		std::array<ComponentMask, MAX_ENTITY> _entityMasks;
		uint32_t _activeEntities = 0;
	};
}