#pragma once
#include "Core/Defines.h"
#include <unordered_map>
#include "Core/Event.h"

namespace CW
{
	// Each component array contains 2 maps, from entity to index, and index to entity, its 3x memory compsumption
	// but 


	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void DestroyEntity(EntityId entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void AddData(EntityId entity, T component)
		{
			size_t newIndex = _size;
			_entityToIndex[entity] = newIndex;
			_indexToEntity[newIndex] = entity;
			_componentArray[newIndex] = component;
			_size++;
		}

		void RemoveData(EntityId entity)
		{
			uint removedIndex = _entityToIndex[entity];
			uint lastIndex = _size - 1;
			_componentArray[removedIndex] = _componentArray[lastIndex];

			EntityId temp = _indexToEntity[lastIndex];
			_entityToIndex[temp] = removedIndex;
			_indexToEntity[removedIndex] = temp;
			
			_entityToIndex.erase(lastIndex);
			_indexToEntity.erase(entity);

			_size--;
		}

		T& GetData(EntityId entity)
		{
			return _componentArray[_entityToIndex[entity]];
		}

		void DestroyEntity(EntityId entity) override
		{
			if (_entityToIndex.contains(entity))
			{
				RemoveData(entity);
			}
		}

	private:

		T _componentArray[MAX_ENTITY]{};
		std::unordered_map<EntityId, uint> _entityToIndex{};
		std::unordered_map<uint, EntityId> _indexToEntity{};

		uint _size;
	};
};