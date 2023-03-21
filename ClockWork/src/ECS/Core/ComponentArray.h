#pragma once
#include "EcsType.h"
#include <unordered_map>

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void AddData(Entity entity, T component)
	{
		size_t newIndex = _size;
		_entityToIndexMap[entity] = newIndex;
		_indexToEntityMap[newIndex] = entity;
		_componentArray[newIndex] = component;
		_size++;
	}

	void RemoveData(Entity entity)
	{
		size_t removedIndex = _entityToIndexMap[entity];
		size_t lastIndex = _size - 1;
		_componentArray[removedIndex] = _componentArray[lastIndex];

		Entity entityOfLastElement = _indexToEntityMap[lastIndex];
		_entityToIndexMap[entityOfLastElement] = removedIndex;
		_indexToEntityMap[removedIndex] = entityOfLastElement;

		_entityToIndexMap.erase(entity);
		_indexToEntityMap.erase(lastIndex);

		_size--;
	}

	T& GetData(Entity entity)
	{
		//assert(!_entityToIndexMap.contains(entity), "cant find");
		return _componentArray[_entityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (_entityToIndexMap.contains(entity))
		{
			RemoveData(entity);
		}
	}

private:

	//std::array<T, MAX_ENTITIES> _componentArray;
	T _componentArray[MAX_ENTITIES]{};
	std::unordered_map<Entity, size_t> _entityToIndexMap{};
	std::unordered_map<size_t, Entity> _indexToEntityMap{};

	size_t _size;
};