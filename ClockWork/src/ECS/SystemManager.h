#pragma once	
#include "core/Defines.h"
#include <set>
#include <unordered_map>
#include <memory>
#include "ISystem.h"


namespace CW
{
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
		void SetMask(ComponentMask mask)
		{
			std::string typeName = typeid(T).name();
			_masks.insert({ typeName, mask });
		}

		void EntityDestroyed(EntityId entity)
		{
			for (const auto& kvp : _systems)
			{
				auto system = kvp.second;

				system->_entities.erase(entity);
			}
		}

		void EntityMaskChanged(EntityId entity, ComponentMask entityMask)
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

		void HandleInput(int input)
		{
			for (auto& system : _systems )
			{
				system.second->GetInput(input);
			}
		}

	private:
		std::unordered_map<std::string, ComponentMask> _masks;
		std::unordered_map<std::string, std::shared_ptr<ISystem>> _systems;
	};
}