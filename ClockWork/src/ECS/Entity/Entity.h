#pragma once
#include "Core/Defines.h"
#include "ECS/ECS.h"
#include <vector>

namespace CW
{
	/* 
		might need to update the EntityID on this element as we are only swapping the index of deleted item with the last element.
		for example if we delete the entity with id 3 in a 6 element of entities, we have to update the entity with id 5 which is the last to deleted index 3.
	*/

	//TODO: fix the issue where the adress of child changes.

	class CW_API Entity
	{
	public:
		static Entity New() 
		{
			auto entityId = ECS::Instance().CreateEntity();
			Entity entity(entityId);
			
			s_lastEntity = &entity;
			s_lastEntityId = entityId;

			return entity;
		}

		static Entity New(Entity& parent)
		{
			auto entityId = ECS::Instance().CreateEntity();
			Entity entity(entityId);
			parent.AddChild(entity);
			
			s_lastEntity = &entity;
			s_lastEntityId = entityId;
			
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
				entity.DestroyChild(child);
			}

			if (entity._parent != nullptr)
			{
				entity._parent->DestroyChild(entity);
			}

			ECS::Instance().DestroyEntity(entity._id);
			s_lastEntity->_id = entity._id;
			s_lastEntityId--;
		}

		EntityId _id;
		std::vector<Entity> _children;
		Entity* _parent = nullptr;

		static Entity* s_lastEntity;
		static EntityId s_lastEntityId;
	};
}