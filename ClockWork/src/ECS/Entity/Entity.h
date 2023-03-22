#pragma once
#include "Core/Defines.h"
#include "ECS/ECS.h"
#include <vector>

namespace CW
{
	class Entity
	{
	public:
		static Entity New() 
		{
			auto entityId = ECS::Instance().CreateEntity();
			Entity entity(entityId);
			return entity;
		}

		static Entity New(Entity& parent)
		{
			auto entityId = ECS::Instance().CreateEntity();
			Entity entity(entityId);
			parent.AddChild(entity);
			return entity;
		}

		void Destroy()
		{
			DestroyRecursive(*this);
		}

		void AddChild(Entity& entity)
		{
			entity._parent = this;
			_children.push_back(entity);
		}

		void DestroyChild(Entity& entity)
		{
			std::remove(_children.begin(), _children.end(), entity);
		}

		bool operator==(const Entity& other)
		{
			return _id == other._id;
		}

		template<typename T>
		void AddComponent(T component) const
		{
			ECS::Instance().AddComponent<T>(_id, component);
		}

		template<typename T>
		void RemoveComponent(T component) const
		{
			ECS::Instance().RemoveComponent<T>(_id, component);
		}

		template<typename T>
		T& GetComponent() const
		{
			ECS::Instance().GetComponent(_id);
		}

	private:
		explicit Entity(EntityId id) : _id(id) {}
		
		void DestroyRecursive(Entity& entity)
		{
			for (auto& child : entity._children)
			{
				DestroyRecursive(child);
			}

			ECS::Instance().DestroyEntity(entity._id);
		}

		EntityId _id;
		std::vector<Entity> _children;
		Entity* _parent = nullptr;
	};
}