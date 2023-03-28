#pragma once
#include "Core/Defines.h"
#include <unordered_map>
#include "Core/Event.h"

namespace CW
{
	//Currently we have 2 unordered maps to keep track of entity and its index in array in case of deletions to keep memory in contact without gaps.
	// this approach is fine but we consume 3x of the memory needed. We can maybe do an in place entityID swap operation in order to sovle this problem.
	// which will resolve it by an O(1) swap operation, but with the drawback of updating the entityID on everyplace that uses it.

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void DestroyEntity(EntityId entity) = 0;

		//Event<int> EntityIdChanged;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void AddData(EntityId entity, T component)
		{
			size_t newIndex = _size;
			/*_entityToIndexMap[entity] = newIndex;
			_indexToEntityMap[newIndex] = entity;*/
			_componentArray[newIndex] = component;
			_size++;
		}

		void RemoveData(EntityId entity)
		{
			uint lastIndex = _size - 1;
			_componentArray[entity] = _componentArray[lastIndex];
			//EntityIdChanged.Invoke(lastIndex, entity);
			_size--;
			
			//size_t removedIndex = _entityToIndexMap[entity];
			//size_t lastIndex = _size - 1;
			//_componentArray[removedIndex] = _componentArray[lastIndex];

			//EntityId entityOfLastElement = _indexToEntityMap[lastIndex];
			//_entityToIndexMap[entityOfLastElement] = removedIndex;
			//_indexToEntityMap[removedIndex] = entityOfLastElement;

			//_entityToIndexMap.erase(entity);
			//_indexToEntityMap.erase(lastIndex);

			//_size--;
		}

		T& GetData(EntityId entity)
		{
			//assert(!_entityToIndexMap.contains(entity), "cant find");
			//return _componentArray[_entityToIndexMap[entity]];
			return _componentArray[entity];
		}

		template<typename T>
		void SetData(EntityId entity, T data)
		{
			_componentArray[entity] = data;
		}

		void DestroyEntity(EntityId entity) override
		{
			RemoveData(entity);

			//if (_entityToIndexMap.contains(entity))
			//{
			//	RemoveData(entity);
			//}
		}

	private:

		T _componentArray[MAX_ENTITY]{};
		//std::unordered_map<EntityId, uint> _entityToIndexMap{};
		//std::unordered_map<uint, EntityId> _indexToEntityMap{};

		uint _size;
	};
};