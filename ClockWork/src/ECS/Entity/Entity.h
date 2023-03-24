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
		using EntitySharedPtr = std::shared_ptr<Entity>;

	public:

		explicit Entity(EntityId id, std::string name) : _id(id), _name(name) { }
		~Entity()
		{
			std::cout << "destroying" << _name << std::endl;
		}

		static EntitySharedPtr Create(std::string name = "")
		{
			auto entityId = ECS::Instance().CreateEntity();

			auto entityPtr = std::make_shared<Entity>(entityId, name);

			s_lastEntity = entityPtr.get();
			s_lastEntityId = entityId;

			return entityPtr;
		} 

		static EntitySharedPtr Create(const EntitySharedPtr& parent, std::string name = "")
		{
			auto entityId = ECS::Instance().CreateEntity();
			auto entityPtr = std::make_shared<Entity>(entityId, name);
			
			parent->AddChild(entityPtr);
			entityPtr->SetParent(parent);

			s_lastEntity = entityPtr.get();
			s_lastEntityId = entityId;

			return entityPtr;
		}

		static void Destroy(EntitySharedPtr& entity)
		{
			for (int i = 0; i < entity->_children.size(); i++)
			{
				Destroy(entity->_children[i]);
			}

			if (entity->_parent != nullptr)
			{
				auto it = std::remove_if
				(
					entity->_parent->_children.begin(),
					entity->_parent->_children.end(),
					[&](auto const& ent) {  return entity->_id == ent->_id; }
				);
				entity->_parent->_children.erase(it, entity->_parent->_children.end());
			}
			entity->_children.clear();

			ECS::Instance().DestroyEntity(entity->_id);
			
			s_lastEntity->_id = entity->_id;
			s_lastEntityId--;
			
			entity = nullptr;
		}


		/*Entity(const Entity& entity)
		{
			std::cout << "copy constructor called\n";
		}*/
		//Entity(Entity&&) = default;

		//Entity& operator=(const Entity& other)
		//{
		//	if (this != &other)
		//	{
		//		_id = other._id;
		//		_children = other._children;
		//		_parent = other._parent;
		//	}
		//	return *this;
		//}

		void AddChild(EntitySharedPtr& entity)
		{
			auto it = std::find(_children.begin(), _children.end(), entity);
			if (it == _children.end())
			{
				_children.push_back(entity);
			}
		}

		void SetParent(const EntitySharedPtr& parent)
		{
			if (_parent != nullptr && _parent == parent)
				return;
			
			// THIS IS AN ACCESS VIOLATION.
			//auto inst = static_cast<EntitySharedPtr>(this);
			
			_parent = parent;
		}

		void DestroyChild(EntityId id)
		{
			auto it = std::find_if(_children.begin(), _children.end(), [&](auto const& e) { return e->_id == id; });

			if (it != _children.end())
			{
				Entity::Destroy(*it);
			}
		}

		void DestroyChild(std::string name)
		{
			auto it = std::remove_if(_children.begin(), _children.end(), [&](auto const& e) { return e->_name == name; });
			if (it != _children.end())
			{
				Entity::Destroy(*it);
			}
		}

		inline bool operator==(const Entity& other)
		{
			return _id == other._id && _name == other._name && _children.size() == other._children.size();
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
		
		EntityId _id = -1;
		std::string _name;
		
		EntitySharedPtr _parent = nullptr;
		std::vector<EntitySharedPtr> _children {};

		static Entity* s_lastEntity;
		static EntityId s_lastEntityId;
	};
}