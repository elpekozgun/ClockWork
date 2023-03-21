#pragma once
#include "EcsType.h"
#include <unordered_map>
#include <memory>
#include "ComponentArray.h"


class CW_API ComponentManager
{
public:
	template<typename T>
	void RegisterComponent()
	{
		std::string typeName = typeid(T).name();

		_componentTypes.insert({ typeName, _nextComponentType });
		_componentArrays.insert({ typeName,std::make_shared<ComponentArray<T>>() });
		_nextComponentType++;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		std::string typeName = typeid(T).name();

		return _componentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->AddData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& kvp : _componentArrays)
		{
			auto const& component = kvp.second;

			component->EntityDestroyed(entity);
		}
	}
		

private:
	std::unordered_map<std::string , ComponentType > _componentTypes{};
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _componentArrays;
	ComponentType _nextComponentType;

	template<typename T> 
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		std::string typeName = typeid(T).name();
		return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);
	}

};