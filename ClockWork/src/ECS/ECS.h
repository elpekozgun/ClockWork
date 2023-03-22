#pragma once
#include "Core/Core.h"
#include "Core/Defines.h"
#include <memory>
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "Core/Singleton.h"

namespace CW
{
	class CW_API ECS final : public Singleton<ECS>
	{
	public:
		ECS(token) {};
		~ECS() {};

	public:
		void Init()
		{
			_entityManager = std::make_unique<EntityManager>();
			_componentManager = std::make_unique<ComponentManager>();
			_systemManager = std::make_unique<SystemManager>();
		}

		EntityId CreateEntity()
		{
			return _entityManager->CreateEntity();
		}

		void DestroyEntity(EntityId entity)
		{
			_entityManager->DestroyEntity(entity);
			_componentManager->EntityDestroyed(entity);
			_systemManager->EntityDestroyed(entity);
		}

		template<typename T>
		void RegisterComponent()
		{
			_componentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(EntityId entity, T component)
		{
			_componentManager->AddComponent(entity, component);

			auto mask = _entityManager->GetMask(entity);
			mask.set(_componentManager->GetComponentType<T>(), true);
			_entityManager->Setmask(entity, mask);

			_systemManager->EntityMaskChanged(entity, mask);
		}

		template<typename T>
		void RemoveComponent(EntityId entity, T component)
		{
			_componentManager->RemoveComponent<T>(entity);

			auto mask = _entityManager->GetMask(entity);
			mask.set(_componentManager->GetComponentType<T>(), false);
			_entityManager->Setmask(entity, mask);
		}

		template<typename T>
		T& GetComponent(EntityId entity)
		{
			return _componentManager->GetComponent<T>(entity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return _componentManager->GetComponentType<T>();
		}

		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return _systemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemMask(ComponentMask mask)
		{
			_systemManager->SetMask<T>(mask);
		}

		void HandleInput(int input)
		{
			_systemManager->HandleInput(input);
		}

	private:
		std::unique_ptr<EntityManager> _entityManager;
		std::unique_ptr<ComponentManager> _componentManager;
		std::unique_ptr<SystemManager> _systemManager;

	};
}