#pragma once
//#include "ECS/Entity/Entity.h"
#include <memory>
#include <string>
#include "ECS/ECS.h"
#include <iostream>
#include <vector>

namespace CW
{
    class Entity;
    using EntityPtr = std::shared_ptr<Entity>;
    using WeakEntityPtr = std::weak_ptr<Entity>;

    class Entity : public std::enable_shared_from_this<Entity>
    {
    public:
        Entity(const std::string& name, EntityPtr parent = nullptr)
            : name(name), parent(parent) {}

        const std::string& getName() const {
            return name;
        }

        EntityPtr getParent() const {
            return parent.lock();
        }

        EntityPtr addChild(const std::string& name) {
            auto child = std::make_shared<Entity>(name, shared_from_this());
            children.push_back(child);
            return child;
        }

        void removeChild(const EntityPtr& child) {
            auto it = std::find(children.begin(), children.end(), child);
            if (it != children.end()) {
                (*it)->parent.reset();
                children.erase(it);
            }
        }

    private:
        std::string name;
        WeakEntityPtr parent;
        std::vector<EntityPtr> children;
    };


    class Scene
    {
    public:
        EntityPtr createEntity(const std::string& name)
        {
            auto entity = std::make_shared<Entity>(name);
            entities.push_back(entity);
            return entity;
        }

        void destroyEntity(const EntityPtr& entity)
        {
            auto it = std::find(entities.begin(), entities.end(), entity);
            if (it != entities.end())
            {
                (*it)->getParent().reset();
                entities.erase(it);
            }
        }
    private:
        std::vector<EntityPtr> entities;
    };
}


//namespace CW
//{
//	class Scene 
//	{
//		class Entity;
//		typedef std::shared_ptr<Entity> EntitySharedPtr;
//		typedef std::weak_ptr<Entity> EntityWeakPtr;
//		typedef std::unique_ptr<Entity> EntityUniquePtr;
//
//	public:
//		EntityWeakPtr AddEntity(std::string name = " ")
//		{
//			auto id = ECS::Instance().CreateEntity();
//			EntitySharedPtr entityPtr = std::make_shared<Entity>(id, name);
//
//			_entities.push_back(entityPtr);
//			
//			return entityPtr;
//		}
//
//		void DestroyEntity(EntityWeakPtr& entity)
//		{
//			//while (entity->_children.size() > 0)
//			//{
//			//	DestroyEntity(entity->_children[0]);
//			//	//entity->_children.erase(entity->_children.begin(), entity->_children.begin() + 1);
//			//}
//
//			auto e = entity.lock();
//
//			for (int i = 0; i < e->_children.size(); i++)
//			{
//				DestroyEntity(e->_children[i]);
//			}
//			
//			e->_children.clear();
//
//			//first clear from scene entities.
//			auto it = std::remove_if
//			(
//				_entities.begin(),
//				_entities.end(),
//				[&](const EntityWeakPtr& ent) {  return e->_id == ent.lock()->_id; }
//			);
//			_entities.erase(it, _entities.end());
//
//			ECS::Instance().DestroyEntity(e->_id);
//
//			////auto parent = entity->_parent.lock();
//			////if (parent != nullptr)
//			////{
//			////	auto it = std::find_if
//			////	(
//			////		parent->_children.begin(),
//			////		parent->_children.end(),
//			////		[&](auto const& ent) {  return entity->_id == ent->_id; }
//			////	);
//			////	*it = nullptr;  //erase(it, parent->_children.end());
//			////}
//
//			//// remove from ecs.
//
//			e = nullptr;
//			////entity.reset();
//		}
//
//
//		void SetParent(EntityWeakPtr& child, EntityWeakPtr& parent)
//		{
//			auto p = parent.lock();
//			auto c = child.lock();
//			//p->_children.push_back(child);
//			p->_children.push_back(child);
//			c->_parent = parent;
//		}
//
//		void ClearParent(EntitySharedPtr& child)
//		{
//			auto p = child->_parent.lock();
//
//			if (p != nullptr)
//			{
//				auto it = std::find_if
//				(
//					p->_children.begin(),
//					p->_children.end(),
//					[&](const EntityWeakPtr& ent) {  return child->_id == ent.lock()->_id; }
//				);
//			}
//			p = nullptr;
//		}
//
//
//	private:
//		class Entity
//		{
//			friend class Scene;
//
//		public:
//			explicit Entity(EntityId id, std::string name) : _id(id), _name(name) 
//			{
//				std::cout << "constructed " << _name << std::endl;
//			}
//			~Entity()
//			{
//				std::cout << "deleting " << _name << std::endl;
//			}
//
//
//		private:
//			EntityId _id = -1;
//			std::string _name;
//			EntityWeakPtr _parent;
//			std::vector<EntityWeakPtr> _children{};
//
//		};
//
//		std::vector<EntitySharedPtr> _entities;
//
//	};
//}