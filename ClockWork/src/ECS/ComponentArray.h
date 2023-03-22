#pragma once
#include "Core/Defines.h"
#include <unordered_map>

namespace CW
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(EntityId entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void AddData(EntityId entity, T component)
		{
			size_t newIndex = _size;
			_entityToIndexMap[entity] = newIndex;
			_indexToEntityMap[newIndex] = entity;
			_componentArray[newIndex] = component;
			_size++;
		}

		void RemoveData(EntityId entity)
		{
			size_t removedIndex = _entityToIndexMap[entity];
			size_t lastIndex = _size - 1;
			_componentArray[removedIndex] = _componentArray[lastIndex];

			EntityId entityOfLastElement = _indexToEntityMap[lastIndex];
			_entityToIndexMap[entityOfLastElement] = removedIndex;
			_indexToEntityMap[removedIndex] = entityOfLastElement;

			_entityToIndexMap.erase(entity);
			_indexToEntityMap.erase(lastIndex);

			_size--;
		}

		T& GetData(EntityId entity)
		{
			//assert(!_entityToIndexMap.contains(entity), "cant find");
			return _componentArray[_entityToIndexMap[entity]];
		}

		void EntityDestroyed(EntityId entity) override
		{
			if (_entityToIndexMap.contains(entity))
			{
				RemoveData(entity);
			}
		}

	private:

		T _componentArray[MAX_ENTITY]{};
		std::unordered_map<EntityId, size_t> _entityToIndexMap{};
		std::unordered_map<size_t, EntityId> _indexToEntityMap{};

		size_t _size;
	};
}