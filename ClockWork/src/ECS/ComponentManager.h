#pragma once
#include "Core/Defines.h"
#include <unordered_map>
#include <memory>
#include "ComponentArray.h"

namespace CW
{
	class CW_API ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			std::string typeName = typeid(T).name();
			if (!_componentTypes.contains(typeName))
			{
				_componentTypes.insert({ typeName, _nextComponentType });
				_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
				_nextComponentType++;
			}
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			std::string typeName = typeid(T).name();

			return _componentTypes[typeName];
		}

		template<typename T>
		void AddComponent(EntityId entity, T component)
		{
			GetComponentArray<T>()->AddData(entity, component);
		}

		template<typename T>
		void RemoveComponent(EntityId entity)
		{ 
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(EntityId entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		template<typename T>
		void SetComponent(EntityId entity, T value)
		{
			GetComponentArray<T>()->SetData(entity, value);
		}


		void DestroyEntity(EntityId entity)
		{
			for (auto const& kvp : _componentArrays)
			{
				auto const& componentArray = kvp.second;

				componentArray->DestroyEntity(entity);
			}
		}


	private:
		std::unordered_map<std::string, ComponentType > _componentTypes{};
		std::unordered_map<std::string, std::shared_ptr<IComponentArray>> _componentArrays;
		ComponentType _nextComponentType;

		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			std::string typeName = typeid(T).name();
			return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);
		}

	};
}