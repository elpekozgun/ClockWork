#pragma once

//#include <queue>
#include <stack>
#include <array>
#include "Core/Defines.h"

namespace CW
{
	/*
		currently we have a queue of entities when we create a new entity we pop a new one from the queue, however when we delete an item, we push that deleted item into the back of the queue
		so we cant use the smaller index again. My aim is to swap the deleted entity with the last one for example
	
		we have a layout for 10 capacity such that :

		0 1 2 3 4 5  and queue as 6,7,8,9

		when we delete 3:

		0 1 2 5 4 -> and queue as 6,7,8,9,3

		next time we add an entity it will be

		0 1 2 5 4 6 and queue as 7,8,9,3
		
		if we use a stack and push indices in reverse orders we get

		0 1 2 3 4 5 and stack as 9 8 7 6 

		if we delete 3 and swap its id with last element we have:

		0 1 2 3 4 and stack as 9 8 7 6 5

		so if we add another item we will have

		0 1 2 3 4 5 and stack as 9 8 7 6 properly indexed.
	
	*/



	class CW_API EntityManager
	{
	public:
		EntityManager()
		{
			for (int i = MAX_ENTITY - 1; i >= 0; i--)
			{
				_availableEntities.push(i);
			}

			/*for (EntityId entity = MAX_ENTITY - 1; entity >= 0; entity--)
			{
				_availableEntities.push(entity);
			}*/
		}

		EntityId CreateEntity()
		{
			//EntityId id = _availableEntities.front();
			EntityId id = _availableEntities.top();
			_availableEntities.pop();
			_activeEntities++;

			return id;
		}

		// we want to set the deleted masks to mask of last added entity as we will be moving it to the spot of deleted one.
		void DestroyEntity(EntityId entity)
		{
			auto lastIndex = _activeEntities - 1;
			//_entityMasks[entity].reset();
			_entityMasks[entity] = _entityMasks[lastIndex];
			_entityMasks[lastIndex].reset();
			
			_availableEntities.push(lastIndex);
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
		//std::queue<EntityId> _availableEntities;
		std::stack<EntityId> _availableEntities;
		std::array<ComponentMask, MAX_ENTITY> _entityMasks;
		uint32_t _activeEntities = 0;
	};
}