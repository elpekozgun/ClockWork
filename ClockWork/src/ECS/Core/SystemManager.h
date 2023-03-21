#pragma once	
#include "EcsType.h"
#include <set>
#include <unordered_map>
#include <memory>


class CW_API SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		std::string typeName = typeid(T).name();
		std::shared_ptr<T> system = std::make_shared<T>();
		_systems.insert({ typeName, system });

		return system;
	}

	template<typename T>
	void SetMask(Mask mask)
	{
		std::string typeName = typeid(T).name();
		_masks.insert({ typeName, mask });
	}

	void EntityDestroyed(Entity entity)
	{
		for (const auto& kvp : _systems)
		{
			auto system = kvp.second;

			system->_entities.erase(entity);
		}
	}

	void EntityMaskChanged(Entity entity, Mask entityMask)
	{
		for (const auto& kvp : _systems)
		{
			const auto& typeName = kvp.first;
			const auto& system = kvp.second;
			const auto& systemMask = _masks[typeName];

			if ((entityMask & systemMask) == systemMask)
				system->_entities.insert(entity);
			else
				system->_entities.erase(entity);

		}
	}

private:
	std::unordered_map<std::string, Mask> _masks;
	std::unordered_map<std::string, std::shared_ptr<System>> _systems;
};