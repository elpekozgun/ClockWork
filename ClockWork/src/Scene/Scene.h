#pragma once
//#include "ECS/Entity/Entity.h"
#include <memory>
#include <string>
#include "ECS/ECS.h"
#include <iostream>
#include <vector>

namespace CW
{
    class Scene
    {
        class Entity;

        using EntityPtr = std::shared_ptr<Entity>;
        using WeakEntityPtr = std::weak_ptr<Entity>;

    public:
        EntityPtr& CreateEntity(const std::string& name, EntityPtr parent = nullptr)
        {
            auto id = ECS::Instance().CreateEntity();

            auto entity = std::make_shared<Entity>(id,name, parent);
            if (parent != nullptr)
                parent->SetChild(entity);

            entities.push_back(entity);
            return entity;
        }

        void DestroyEntity(EntityPtr& entity)
        {
            auto it = std::find(entities.begin(), entities.end(), entity);
            if (it != entities.end())
            {
                DestroyRecursive(entity);
            }
            entity.reset();
        }

        void DestroyRecursive(EntityPtr& entity)
        {
            //first remove entity from its parent children.
            if (auto parent = entity->GetParent())
            {
                parent->RemoveChild(entity);
            }

            // reset entity parent
            entity->_parent = WeakEntityPtr();

            // do the same for everychildren in entity.
            while (!entity->_children.empty())
            {
                auto child = entity->_children.back();
                entity->_children.pop_back();
                DestroyRecursive(child);
                //auto c = child.lock();
                //DestroyRecursive(c);
            }
            
            // remove entity from scene entities
            auto itr = std::find(entities.begin(), entities.end(), entity);
            if (itr != entities.end())
            {
                entities.erase(itr);
            }

            // finally destroy it
            entity.reset();
        }

    private:
        std::vector<EntityPtr> entities;

        class Entity : public std::enable_shared_from_this<Entity>
        {
            friend class Scene;

        public:
            Entity(EntityId id, const std::string& name, const EntityPtr& parent = nullptr)
                : _id(id),_name(name), _parent(parent) 
            {
                std::cout << "creating " << _id << " " << _name << "\n";
            }

            ~Entity()
            {
                std::cout << "destroying " << _id << " " << _name << "\n";
            }


            const std::string& GetName() const {
                return _name;
            }

            EntityPtr GetParent() const {
                return _parent.lock();
            }

            void SetParent(EntityPtr& parent)
            {
                _parent = parent;
                parent->_children.push_back(shared_from_this());
            }

            void SetChild(EntityPtr& child)
            {
                if (child->GetParent() != nullptr)
                {
                    child->GetParent()->RemoveChild(child);
                }
                auto a = shared_from_this();
                child->SetParent(a);
            }

            void RemoveChild(const EntityPtr& child) 
            {
                //auto it = std::find_if(_children.begin(), _children.end(), [&](const auto& a) { return a.lock() == child; });
                auto it = std::find(_children.begin(), _children.end(), child);
                if (it != _children.end()) 
                {
                    //auto c = it->lock();
                    //c->_parent = WeakEntityPtr();
                    (*it)->_parent = WeakEntityPtr();
                    _children.erase(it);
                }
            }

            bool operator==(const WeakEntityPtr& other)
            {
                auto t = other.lock();

                return shared_from_this() == t;
            }

            //void DestroyChildren()
            //{
            //    while(!_children.empty()) 
            //    {
            //        auto child = _children.back();
            //        child->DestroyChildren();
            //        _children.pop_back();
            //    }
            //}

        private:
            EntityId _id;
            std::string _name;
            WeakEntityPtr _parent;
            std::vector<EntityPtr> _children;
        };

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