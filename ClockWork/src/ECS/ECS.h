#pragma once

#include <memory>
#include <bitset>
#include <functional>
#include <tuple>

#include "Core/Core.h"
#include "Core/Defines.h"
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "Core/Singleton.h"
#include "Component.h"

namespace CW 
{
	class CW_API ECS final : public Singleton<ECS>
	{
	public:
		ECS(token) {};
		~ECS() {};

		void Init()
		{
			_entityManager = std::make_unique<EntityManager>();
			_componentManager = std::make_unique<ComponentManager>();
			_systemManager = std::make_unique<SystemManager>();
			_assetManager = std::make_unique<AssetManager>();
		}

		EntityId CreateEntity()
		{
			return _entityManager->CreateEntity();
		}

		void DestroyEntity(EntityId entity)
		{
			_entityManager->DestroyEntity(entity);
			_componentManager->DestroyEntity(entity);
			_systemManager->DestroyEntity(entity);
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

			_systemManager->EntityMaskChanged(entity, mask);
		}

		template<typename T>
		T& GetComponent(EntityId entity)
		{
			return _componentManager->GetComponent<T>(entity);
		}
		

		template<typename T>
		void SetComponent(EntityId entity, T component)
		{
			_componentManager->SetComponent(entity, component);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return _componentManager->GetComponentType<T>();
		}

		template<typename T = ISystem, typename... C>
		std::shared_ptr<T> RegisterSystem()
		{
			auto system = _systemManager->RegisterSystem<T>();

			ComponentMask mask;
			RegisterComponents<C...>(mask);
			SetSystemMask<T>(mask);

			return system;
		}

		template<typename... C>
		void RegisterComponents(ComponentMask& mask)
		{
			(RegisterComponent<C>(mask), ...);
		}

		template<typename T>
		T& GetAsset(uint assetId)
		{
			return _assetManager->GetAsset<T>(assetId);
		}

		template<typename T>
		uint AddAsset(T& asset)
		{
			return _assetManager->AddAsset<T>(asset);
		}

		template<typename T = MeshComponent>
		void MakeMeshInstanced(unsigned int id, std::vector<glm::mat4>& transforms)
		{
			_assetManager->MakeInstanced<T>(id, transforms);
		}


		CameraComponent& GetSingleton_Camera()
		{
			return _camera;
		}


	private:
		template<typename... C>
		void RegisterComponent(ComponentMask& mask)
		{
			_componentManager->RegisterComponent<C...>();
			auto a = _componentManager->GetComponentType<C...>();
			mask.set(a, true);
		}

		template<typename T>
		void SetSystemMask(ComponentMask mask)
		{
			_systemManager->SetSystemMask<T>(mask);
		}

		std::unique_ptr<EntityManager> _entityManager;
		std::unique_ptr<ComponentManager> _componentManager;
		std::unique_ptr<SystemManager> _systemManager;
		std::unique_ptr<AssetManager> _assetManager;

		CameraComponent _camera;
	};
}